#include "PhysicsSystem.h"
#include "../Entity.h"
#include "../../Input/Keyboard.h"
#include "../../Analysis/Logger.h"
#include "../../Input/InputDevice.h"
#include "../EntityComponentSystem.h"
#include "../../Utilities/Math/CCMath.h"
#include "../Components/ComponentHeaders.h"
#include "../../Utilities/Containers/Octree.h"
#include "../../Graphics/RenderPipeline.h"


namespace CCE::ECS::Systems
{
	using ECS = EntityComponentSystem;

	void PhysicsSystem::StartUp()
	{
		ECS::Instance->RegisterSystem<PhysicsSystem>();
		REGISTER_INPUT_CALLBACK;
		LOGC("PhysicsSystem initialized!", COLOR_BLUE);
	}

	void PhysicsSystem::ShutDown()
	{
		LOGC("PhysicsSystem uninitialized!", COLOR_BLUE);
	}

	void PhysicsSystem::RegisterEntity(long long entity)
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;

		mEntities.insert(entity);
	}

	void PhysicsSystem::UpdateSystem()
	{
		if (pause) { return; }	// Pause simulation

		start = Time::Now();
		Step();
		end = Time::Now();
		PhysicsCalcDuration = Time::GetDurationInMilliSec(start, end);
	}

	void PhysicsSystem::TogglePause()
	{
		pause = !pause;
	}

	void PhysicsSystem::Step()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;

		for (long long id : mEntities)
		{
			Entity e = Entity(id);

			auto* tf = e.GetComponent<Transform>();
			auto* rb = e.GetComponent<Rigidbody>();

			// Update rigidbody internal acceleration
			
			rb->UpdateRigidbody();
			
			// Take rigidbody velocity to update position
			DirectX::XMFLOAT3 newPos{};
			auto _newPos = DirectX::XMVectorAdd({ tf->Position().x, tf->Position().y, tf->Position().z },
				DirectX::XMVectorScale(XMLoadFloat3(&rb->velocity), Time::deltaTime));

			DirectX::XMStoreFloat3(&newPos, _newPos);
			tf->SetTranslation(std::move(newPos));		
		}

		// Now do collision testing
		BroadPhaseCollisionDetection();
		MidPhaseCollisionDetection();
		NarrowPhaseCollisionDetection();
	}

	/// <summary>
	/// The broad phase of the collision system.
	/// Constructs a spatial tree (octree) and inserts all objects in it.
	/// </summary>
	void PhysicsSystem::BroadPhaseCollisionDetection()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::Containers;
		using namespace CCE::ECS::Components;
		
		Octree<Entity> bspTree({8,8,8}, 15, 15);
		DirectX::XMFLOAT3 boundingVolume;

		// Add all collidable objects
		for (long long id : mEntities)
		{
			Entity e = Entity(id);

			if(e.GetComponent<BoxCollider>() != nullptr)
			{
				auto* _col = e.GetComponent<BoxCollider>();
				bspTree.Insert(e, e.GetComponent<Transform>()->Position(), _col->GetBoundingBox());
			}
			else if (e.GetComponent<SphereCollider>() != nullptr)
			{
				auto* _col = e.GetComponent<SphereCollider>();
				bspTree.Insert(e, e.GetComponent<Transform>()->Position(), _col->GetBoundingBox());
			}
			else
			{
				DASSERT(false, "Invalid bounding volume type.");
			}
		}
		
		// Insert all collision tuples into the global frame collisions
		bspTree.OperateOnContents(
			[&](std::vector<OctreeEntry<Entity>>& data) 
			{
				for (auto i = data.begin(); i != data.end(); ++i)
				{
					for (auto j = data.begin(); j != data.end(); ++j)
					{
						if (j->pObjRef.Id == i->pObjRef.Id) { continue; } // Disallow self collision!

						auto first = i->pObjRef.Id < j->pObjRef.Id ? i->pObjRef.Id : j->pObjRef.Id;
						auto second = i->pObjRef.Id < j->pObjRef.Id ? j->pObjRef.Id : i->pObjRef.Id;

						FrameCollisionCandidates.insert(Physics::CollisionInfo(first, second));
					}
				}
			}
		);

		// @TODO: Multithread every tree node (?)
	}

	/// <summary>
	/// The mid phase of the collision system.
	/// Checks for AABB collision of 
	/// </summary>
	void PhysicsSystem::MidPhaseCollisionDetection()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;

		for (auto& collisionPair : FrameCollisionCandidates)
		{
			Entity first(collisionPair.first);
			Entity second(collisionPair.second);

			// Do not check for self collision
			DASSERT(first != second,
				"Checking for self collision is invalid! Both colliders have the same pointer!");

			BoxCollider firstBox;
			BoxCollider secondBox;
			SphereCollider firstSphere;
			SphereCollider secondSphere;
			if (first.TryGetComponent<BoxCollider>(firstBox) && second.TryGetComponent<BoxCollider>(secondBox))
			{
				// First AABB check
				if (CollideAABB(first.GetComponent<Transform>(), first.GetComponent<BoxCollider>(),
					second.GetComponent<Transform>(), second.GetComponent<BoxCollider>()))
				{
					AABBAABBFrameCollisions.insert(collisionPair);
				}
			}
			else if (first.TryGetComponent<BoxCollider>(firstBox) && second.TryGetComponent<SphereCollider>(secondSphere))
			{
				// First AABB check
				if (CollideAABB(first.GetComponent<Transform>()->Position(), firstBox.GetBoundingBox(),
					second.GetComponent<Transform>()->Position(), secondSphere.GetBoundingBox()))
				{
					SphereAABBFrameCollisions.insert(collisionPair);
				}
			}
			else if (first.TryGetComponent<SphereCollider>(firstSphere) && second.TryGetComponent<BoxCollider>(secondBox))
			{
				// First AABB check
				if (CollideAABB(first.GetComponent<Transform>()->Position(), firstSphere.GetBoundingBox(),
					second.GetComponent<Transform>()->Position(), secondBox.GetBoundingBox()))
				{
					SphereAABBFrameCollisions.insert(collisionPair);
				}
			}
			else if (first.TryGetComponent<SphereCollider>(firstSphere) && second.TryGetComponent<SphereCollider>(secondSphere))
			{
				// First AABB check
				if (CollideSpheres(first.GetComponent<Transform>(), &firstSphere,
					second.GetComponent<Transform>(), &secondSphere))
				{
					SphereSphereFrameCollisions.insert(collisionPair);
				}
			}
		}
	}

	/// <summary>
	/// The narrow phase of the collision system.
	/// Detects collisions and resolves them.
	/// </summary>
	void PhysicsSystem::NarrowPhaseCollisionDetection() const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		// Create const buffers and somehow do that with respect to the amount of collisions!
		using RP = Graphics::RenderPipeline;
		using namespace Microsoft::WRL;

		ComPtr<ID3D11Buffer> inputDataBuf{};
		ComPtr<ID3D11Buffer> outputDataBuf{};
		ComPtr<ID3D11Buffer> sharedOutputDataBuf{};
		ComPtr<ID3D11ShaderResourceView> inputDataView{};
		ComPtr<ID3D11UnorderedAccessView> outputDataView{};

		CSInputData inData{};
		// Fill data
		FillConstantBuffer(inData);

		if (inData.cpd.size() == 0)
		{
			return;
		}

		D3D11_BUFFER_DESC csInputDataDesc{};
		csInputDataDesc.Usage = D3D11_USAGE_DYNAMIC;
		csInputDataDesc.ByteWidth = sizeof(CollisionPairInData) * inData.cpd.size();
		csInputDataDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		csInputDataDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		csInputDataDesc.StructureByteStride = sizeof(CollisionPairInData);
		csInputDataDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = inData.cpd.data();
		initData.SysMemPitch = 0u;	// 2D & 3D Textures only
		initData.SysMemSlicePitch = 0u; // 3D Textures only

		HRESULT hr = RP::Instance->GetDevicePtr()->CreateBuffer(&csInputDataDesc, &initData, inputDataBuf.GetAddressOf());
		DASSERT(hr == S_OK, "Failed creating compute shader shared input buffer description.");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = inData.cpd.size();

		hr = RP::Instance->GetDevicePtr()->CreateShaderResourceView(inputDataBuf.Get(), &srvDesc, inputDataView.GetAddressOf());
		DASSERT(hr == S_OK, "Failed creating compute shader shared input buffer.");

		// RW Buffer for output
		D3D11_BUFFER_DESC outputDesc{};
		outputDesc.Usage = D3D11_USAGE_DEFAULT;
		outputDesc.ByteWidth = sizeof(CollisionPairOutData) * inData.cpd.size();
		outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		outputDesc.CPUAccessFlags = 0;
		outputDesc.StructureByteStride = sizeof(CollisionPairOutData);
		outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = (RP::Instance->GetDevicePtr()->CreateBuffer(&outputDesc, 0u, outputDataBuf.GetAddressOf()));
		DASSERT(hr == S_OK, "Failed creating compute shader output buffer.");

		D3D11_BUFFER_DESC sharedOutputDesc{};
		sharedOutputDesc.Usage = D3D11_USAGE_STAGING;
		sharedOutputDesc.ByteWidth = sizeof(CollisionPairOutData) * inData.cpd.size();
		sharedOutputDesc.BindFlags = 0;
		sharedOutputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		sharedOutputDesc.StructureByteStride = sizeof(CollisionPairOutData);
		sharedOutputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = (RP::Instance->GetDevicePtr()->CreateBuffer(&sharedOutputDesc, 0u, sharedOutputDataBuf.GetAddressOf()));
		DASSERT(hr == S_OK, "Failed creating compute shader shared output buffer.");

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
		uavDesc.Buffer.FirstElement = 0u;
		uavDesc.Buffer.Flags = 0u;
		uavDesc.Buffer.NumElements = inData.cpd.size();
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

		hr = RP::Instance->GetDevicePtr()->CreateUnorderedAccessView(outputDataBuf.Get(), &uavDesc, outputDataView.GetAddressOf());
		DASSERT(hr == S_OK, "Failed creating compute shader unordered access view for output buffer.");

		ComPtr<ID3DBlob> pBlob{};
		hr = D3DReadFileToBlob(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/compute-shader/CollisionDetectionResolve.cso", pBlob.GetAddressOf());
		DASSERT(hr == S_OK, "Failed reading the compute shader.");
		// Check shader version for older cpus / gpus (my laptop!! :O)
		hr = RP::Instance->GetDevicePtr()->CreateComputeShader(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, pComputeShader.GetAddressOf());
		DASSERT(hr == S_OK, "Failed creating the compute shader resource.");

		RP::Instance->GetDeviceContextPtr()->CSSetShader(pComputeShader.Get(), nullptr, 0u);

		RP::Instance->GetDeviceContextPtr()->CSSetShaderResources(0u, 1u, inputDataView.GetAddressOf());
		RP::Instance->GetDeviceContextPtr()->CSSetUnorderedAccessViews(0u, 1u, outputDataView.GetAddressOf(), 0u);

		RP::Instance->GetDeviceContextPtr()->Dispatch(1u, 1u, 1u);

		// Unbind the input textures from the CS
		ID3D11ShaderResourceView* nullSRV[] = { NULL };
		RP::Instance->GetDeviceContextPtr()->CSSetShaderResources(0u, 1u, nullSRV);

		// Unbind output from compute shader
		ID3D11UnorderedAccessView* nullUAV[] = { NULL };
		RP::Instance->GetDeviceContextPtr()->CSSetUnorderedAccessViews(0u, 1u, nullUAV, 0u);

		// Disable Compute Shader
		RP::Instance->GetDeviceContextPtr()->CSSetShader(nullptr, nullptr, 0u);

		auto* sodb = sharedOutputDataBuf.Get();
		auto* odb = outputDataBuf.Get();

		//D3DX11SaveTextureToFile

		RP::Instance->GetDeviceContextPtr()->CopyResource(sharedOutputDataBuf.Get(), outputDataBuf.Get());
		
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		hr = RP::Instance->GetDeviceContextPtr()->Map(sharedOutputDataBuf.Get(), 0u, D3D11_MAP::D3D11_MAP_READ, 0u, &mappedResource);
		DASSERT(hr == S_OK, "Failed mapping compute shader shared output buffer.");

		if (SUCCEEDED(hr))
		{
			CSOutputData* dataView = reinterpret_cast<CSOutputData*>(mappedResource.pData);

			if(dataView->cpd.size() > 0)
				ApplyComputeShaderData(dataView);

			RP::Instance->GetDeviceContextPtr()->Unmap(sharedOutputDataBuf.Get(), 0u);
		}
	}

	void PhysicsSystem::FillConstantBuffer(CCE::ECS::Systems::PhysicsSystem::CSInputData& inData) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		// AABB-AABB-Collisions
		for (auto& collisionPair : AABBAABBFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			BoxCollider* colBoxA = a.GetComponent<BoxCollider>();
			BoxCollider* colBoxB = b.GetComponent<BoxCollider>();

			CollisionInfo cInfo = collisionPair;

			if (!CollideInfoAABB(tfA, colBoxA, tfB, colBoxB, cInfo))
			{
				continue;
			}

			CollisionPairInData pairData{};
			pairData.positionA = { tfA->Position().x, tfA->Position().y, tfA->Position().z , 1.0f };
			pairData.positionB = { tfB->Position().x, tfB->Position().y, tfB->Position().z , 1.0f };
			pairData.colliderDimA = { colBoxA->Width, colBoxA->Height, colBoxA->Length, 1.0f };
			pairData.colliderDimB = { colBoxB->Width, colBoxB->Height, colBoxB->Length, 1.0f };
			pairData.velocityA = { rbA->velocity.x, rbA->velocity.y, rbA->velocity.z,  1.0f };
			pairData.velocityB = { rbB->velocity.x, rbB->velocity.y, rbB->velocity.z,  1.0f };
			pairData.angularVelocityA = { rbA->angularVelocity.x, rbA->angularVelocity.y, rbA->angularVelocity.z, 1.0f };
			pairData.angularVelocityB = { rbB->angularVelocity.x, rbB->angularVelocity.y, rbB->angularVelocity.z, 1.0f };
			pairData.bouncinessA = { rbA->bounciness, rbA->bounciness, rbA->bounciness, rbA->bounciness };
			pairData.bouncinessB = { rbB->bounciness, rbB->bounciness, rbB->bounciness, rbB->bounciness };
			pairData.massA = { rbA->mass, rbA->mass, rbA->mass, rbA->mass };
			pairData.massB = { rbB->mass, rbB->mass, rbB->mass, rbB->mass };
			pairData.shapeA = { 0, 0, 0, 0 };
			pairData.shapeB = { 0, 0, 0, 0 };

			pairData.collisionPointA = { cInfo.contactPoint.collisionPointFirst.x,
				cInfo.contactPoint.collisionPointFirst.y, cInfo.contactPoint.collisionPointFirst.z, 1.0f };
			pairData.collisionPointB = { cInfo.contactPoint.collisionPointSecond.x,
				cInfo.contactPoint.collisionPointSecond.y, cInfo.contactPoint.collisionPointSecond.z, 1.0f };
			pairData.collisionNormal = { cInfo.contactPoint.collisionNormal.x,
				cInfo.contactPoint.collisionNormal.y, cInfo.contactPoint.collisionNormal.z, 1.0f };
			pairData.penetration = { cInfo.contactPoint.penetration, cInfo.contactPoint.penetration,
				cInfo.contactPoint.penetration, cInfo.contactPoint.penetration };

			inData.cpd.push_back(std::move(pairData));
		}

		// Sphere-AABB-Collisions
		for (auto& collisionPair : SphereAABBFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			SphereCollider* colSphereA = a.GetComponent<SphereCollider>();
			SphereCollider* colSphereB = b.GetComponent<SphereCollider>();

			BoxCollider* colBoxA = a.GetComponent<BoxCollider>();
			BoxCollider* colBoxB = b.GetComponent<BoxCollider>();

			CollisionInfo cInfo = collisionPair;

			// Mixed collision
			auto* box = colBoxA == nullptr ? colBoxB : colBoxA;
			auto* sphere = colSphereA == nullptr ? colSphereB : colSphereA;

			if (!CollideInfoSphereAABB(tfB, box, tfA, sphere, cInfo))
			{
				continue;
			}

			CollisionPairInData pairData{};
			pairData.positionA = { tfB->Position().x, tfB->Position().y, tfB->Position().z , 1.0f };
			pairData.positionB = { tfA->Position().x, tfA->Position().y, tfA->Position().z , 1.0f };
			pairData.colliderDimA = { box->Width, box->Height, box->Length, 1.0f };
			pairData.colliderDimB = { sphere->Radius, sphere->Radius, sphere->Radius, 1.0f };
			pairData.velocityA = { rbB->velocity.x, rbB->velocity.y, rbB->velocity.z,  1.0f };
			pairData.velocityB = { rbA->velocity.x, rbA->velocity.y, rbA->velocity.z,  1.0f };
			pairData.angularVelocityA = { rbB->angularVelocity.x, rbB->angularVelocity.y, rbB->angularVelocity.z, 1.0f };
			pairData.angularVelocityB = { rbA->angularVelocity.x, rbA->angularVelocity.y, rbA->angularVelocity.z, 1.0f };
			pairData.bouncinessA = { rbB->bounciness, rbB->bounciness, rbB->bounciness, rbB->bounciness };
			pairData.bouncinessB = { rbA->bounciness, rbA->bounciness, rbA->bounciness, rbA->bounciness };
			pairData.massB = { rbA->mass, rbA->mass, rbA->mass, rbA->mass };
			pairData.massA = { rbB->mass, rbB->mass, rbB->mass, rbB->mass };
			pairData.shapeA = { 0, 0, 0, 0 };
			pairData.shapeB = { 1, 1, 1, 1 };

			pairData.collisionPointA = { cInfo.contactPoint.collisionPointSecond.x,
				cInfo.contactPoint.collisionPointSecond.y, cInfo.contactPoint.collisionPointSecond.z, 1.0f };
			pairData.collisionPointB = { cInfo.contactPoint.collisionPointFirst.x,
				cInfo.contactPoint.collisionPointFirst.y, cInfo.contactPoint.collisionPointFirst.z, 1.0f };
			pairData.collisionNormal = { cInfo.contactPoint.collisionNormal.x,
				cInfo.contactPoint.collisionNormal.y, cInfo.contactPoint.collisionNormal.z, 1.0f };
			pairData.penetration = { cInfo.contactPoint.penetration, cInfo.contactPoint.penetration,
				cInfo.contactPoint.penetration, cInfo.contactPoint.penetration };

			inData.cpd.push_back(std::move(pairData));
		}

		// Sphere-Sphere-Collisions
		for (auto& collisionPair : SphereSphereFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			SphereCollider* colSphereA = a.GetComponent<SphereCollider>();
			SphereCollider* colSphereB = b.GetComponent<SphereCollider>();

			CollisionInfo cInfo = collisionPair;

			if (!CollideInfoSpheres(tfA, colSphereA, tfB, colSphereB, cInfo))
			{
				continue;
			}

			CollisionPairInData pairData{};
			pairData.positionA = { tfA->Position().x, tfA->Position().y, tfA->Position().z , 1.0f };
			pairData.positionB = { tfB->Position().x, tfB->Position().y, tfB->Position().z , 1.0f };
			pairData.colliderDimA = { colSphereA->Radius, colSphereA->Radius, colSphereA->Radius, 1.0f };
			pairData.colliderDimB = { colSphereB->Radius, colSphereB->Radius, colSphereB->Radius, 1.0f };
			pairData.velocityA = { rbA->velocity.x, rbA->velocity.y, rbA->velocity.z,  1.0f };
			pairData.velocityB = { rbB->velocity.x, rbB->velocity.y, rbB->velocity.z,  1.0f };
			pairData.angularVelocityA = { rbA->angularVelocity.x, rbA->angularVelocity.y, rbA->angularVelocity.z, 1.0f };
			pairData.angularVelocityB = { rbB->angularVelocity.x, rbB->angularVelocity.y, rbB->angularVelocity.z, 1.0f };
			pairData.bouncinessA = { rbA->bounciness, rbA->bounciness, rbA->bounciness, rbA->bounciness };
			pairData.bouncinessB = { rbB->bounciness, rbB->bounciness, rbB->bounciness, rbB->bounciness };
			pairData.massA = { rbA->mass, rbA->mass, rbA->mass, rbA->mass };
			pairData.massB = { rbB->mass, rbB->mass, rbB->mass, rbB->mass };
			pairData.shapeA = { 1, 1, 1, 1 };
			pairData.shapeB = { 1, 1, 1, 1 };

			pairData.collisionPointA = { cInfo.contactPoint.collisionPointFirst.x,
				cInfo.contactPoint.collisionPointFirst.y, cInfo.contactPoint.collisionPointFirst.z, 1.0f };
			pairData.collisionPointB = { cInfo.contactPoint.collisionPointSecond.x,
				cInfo.contactPoint.collisionPointSecond.y, cInfo.contactPoint.collisionPointSecond.z, 1.0f };
			pairData.collisionNormal = { cInfo.contactPoint.collisionNormal.x,
				cInfo.contactPoint.collisionNormal.y, cInfo.contactPoint.collisionNormal.z, 1.0f };
			pairData.penetration = { cInfo.contactPoint.penetration, cInfo.contactPoint.penetration,
				cInfo.contactPoint.penetration, cInfo.contactPoint.penetration };

			inData.cpd.push_back(std::move(pairData));
		}
	}

	void PhysicsSystem::ApplyComputeShaderData(CCE::ECS::Systems::PhysicsSystem::CSOutputData* dataView) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		// Update positions and velocities
		size_t resultID = 0;
		for (auto& collisionPair : AABBAABBFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			tfA->SetTranslation(
				{
					dataView->cpd[resultID].newPositionA.x,
					dataView->cpd[resultID].newPositionA.y,
					dataView->cpd[resultID].newPositionA.z
				}
			);

			tfB->SetTranslation(
				{
					dataView->cpd[resultID].newPositionB.x,
					dataView->cpd[resultID].newPositionB.y,
					dataView->cpd[resultID].newPositionB.z
				}
			);

			rbA->ApplyLinearImpulse(
				{
					dataView->cpd[resultID].linearImpulseA.x,
					dataView->cpd[resultID].linearImpulseA.y,
					dataView->cpd[resultID].linearImpulseA.z
				}
			);

			rbB->ApplyLinearImpulse(
				{
					dataView->cpd[resultID].linearImpulseB.x,
					dataView->cpd[resultID].linearImpulseB.y,
					dataView->cpd[resultID].linearImpulseB.z
				}
			);

			rbA->ApplyAngularImpulse(
				{
					dataView->cpd[resultID].angularImpulseA.x,
					dataView->cpd[resultID].angularImpulseA.y,
					dataView->cpd[resultID].angularImpulseA.z
				}
			);
			rbB->ApplyAngularImpulse(
				{
					dataView->cpd[resultID].angularImpulseB.x,
					dataView->cpd[resultID].angularImpulseB.y,
					dataView->cpd[resultID].angularImpulseB.z
				}
			);

			// @TODO: Update angular impulse
			// Currently not necessary!

			++resultID;
		}

		for (auto& collisionPair : SphereAABBFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			tfA->SetTranslation(
				{
					dataView->cpd[resultID].newPositionA.x,
					dataView->cpd[resultID].newPositionA.y,
					dataView->cpd[resultID].newPositionA.z
				}
			);

			tfB->SetTranslation(
				{
					dataView->cpd[resultID].newPositionB.x,
					dataView->cpd[resultID].newPositionB.y,
					dataView->cpd[resultID].newPositionB.z
				}
			);

			rbA->ApplyLinearImpulse(
				{
					dataView->cpd[resultID].linearImpulseA.x,
					dataView->cpd[resultID].linearImpulseA.y,
					dataView->cpd[resultID].linearImpulseA.z
				}
			);

			rbB->ApplyLinearImpulse(
				{
					dataView->cpd[resultID].linearImpulseB.x,
					dataView->cpd[resultID].linearImpulseB.y,
					dataView->cpd[resultID].linearImpulseB.z
				}
			);

			rbA->ApplyAngularImpulse(
				{
					dataView->cpd[resultID].angularImpulseA.x,
					dataView->cpd[resultID].angularImpulseA.y,
					dataView->cpd[resultID].angularImpulseA.z
				}
			);
			rbB->ApplyAngularImpulse(
				{
					dataView->cpd[resultID].angularImpulseB.x,
					dataView->cpd[resultID].angularImpulseB.y,
					dataView->cpd[resultID].angularImpulseB.z
				}
			);

			// @TODO: Update angular impulse
			// Currently not necessary!

			++resultID;
		}

		for (auto& collisionPair : SphereSphereFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			tfA->SetTranslation(
				{
					dataView->cpd[resultID].newPositionA.x,
					dataView->cpd[resultID].newPositionA.y,
					dataView->cpd[resultID].newPositionA.z
				}
			);

			tfB->SetTranslation(
				{
					dataView->cpd[resultID].newPositionB.x,
					dataView->cpd[resultID].newPositionB.y,
					dataView->cpd[resultID].newPositionB.z
				}
			);

			rbA->ApplyLinearImpulse(
				{
					dataView->cpd[resultID].linearImpulseA.x,
					dataView->cpd[resultID].linearImpulseA.y,
					dataView->cpd[resultID].linearImpulseA.z
				}
			);

			rbB->ApplyLinearImpulse(
				{
					dataView->cpd[resultID].linearImpulseB.x,
					dataView->cpd[resultID].linearImpulseB.y,
					dataView->cpd[resultID].linearImpulseB.z
				}
			);

			rbA->ApplyAngularImpulse(
				{
					dataView->cpd[resultID].angularImpulseA.x,
					dataView->cpd[resultID].angularImpulseA.y,
					dataView->cpd[resultID].angularImpulseA.z
				}
			);
			rbB->ApplyAngularImpulse(
				{
					dataView->cpd[resultID].angularImpulseB.x,
					dataView->cpd[resultID].angularImpulseB.y,
					dataView->cpd[resultID].angularImpulseB.z
				}
			);

			// @TODO: Update angular impulse
			// Currently not necessary!

			++resultID;
		}
	}

	void PhysicsSystem::ApplyAngularTransformations(CCE::ECS::Components::Rigidbody* rbA, CCE::ECS::Components::Transform* tfA, 
		CCE::ECS::Components::Rigidbody* rbB, CCE::ECS::Components::Transform* tfB) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;
		
		if (applyAngularImpulse)
		{
			XMFLOAT3 rotationFirst{};
			XMFLOAT3 rotationSecond{};

			XMStoreFloat3(&rotationFirst, XMVectorAdd(XMLoadFloat3(&tfA->Rotation()), XMLoadFloat3(&rbA->angularVelocity)));
			XMStoreFloat3(&rotationSecond, XMVectorAdd(XMLoadFloat3(&tfB->Rotation()), XMLoadFloat3(&rbB->angularVelocity)));

			tfA->SetRotation(rotationFirst);
			tfB->SetRotation(rotationSecond);
		}
	}

	void PhysicsSystem::InputCallback(const Input::Mouse* mouse, const Input::Keyboard* keyboard, const Input::Controller* controller)
	{
		using namespace Input;
		if (keyboard->keys[(int)InputDevice::Keycode::SPACE] == Keyboard::ButtonState::PRESSED ||
			controller->RSouth == InputDevice::ButtonState::PRESSED)
		{
			TogglePause();
		}

	}

	void PhysicsSystem::ResolveCollisionImpulse(CCE::ECS::Components::Rigidbody* rbA, CCE::ECS::Components::Transform* tfA,
		CCE::ECS::Components::Collider* colA, CCE::ECS::Components::Rigidbody* rbB, CCE::ECS::Components::Transform* tfB, 
		CCE::ECS::Components::Collider* colB, CCE::Physics::CollisionInfo& cInfo, float totalInverseMass) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		// Set angular velocity and calc contact velocity
		XMVECTOR relativeA = XMLoadFloat3(&cInfo.contactPoint.collisionPointFirst) - XMLoadFloat3(&tfA->Position());
		XMVECTOR relativeB = XMLoadFloat3(&cInfo.contactPoint.collisionPointFirst) - XMLoadFloat3(&tfB->Position());

		// Calculate angular velocity
		XMVECTOR angVelA = XMVector3Cross(XMLoadFloat3(&rbA->angularVelocity), relativeA);
		XMVECTOR angVelB = XMVector3Cross(XMLoadFloat3(&rbB->angularVelocity), relativeB);

		XMVECTOR fullVelocityA = XMLoadFloat3(&rbA->velocity) + angVelA;
		XMVECTOR fullVelocityB = XMLoadFloat3(&rbB->velocity) + angVelB;

		XMVECTOR contactVelocity = fullVelocityB - fullVelocityA;

		XMFLOAT3 impulseForce{};
		XMStoreFloat3(&impulseForce, XMVector3Dot(contactVelocity, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)));

		// Calculate inertia
		XMVECTOR inertiaA = XMVector3Cross(XMVector3Transform(XMVector3Cross(relativeA, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)),
			XMLoadFloat3x3(&rbA->InertiaTensor(colA))), relativeA);

		XMVECTOR inertiaB = XMVector3Cross(XMVector3Transform(XMVector3Cross(relativeB, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)),
			XMLoadFloat3x3(&rbB->InertiaTensor(colB))), relativeB);

		XMFLOAT3 angularEffect{};
		XMStoreFloat3(&angularEffect, XMVector3Dot(inertiaA + inertiaB, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)));

		float damping = CCE::Math::CCMath::Clamp01((rbA->bounciness + rbB->bounciness) / 2.0f);
		float j = (-(1.0f + damping) * impulseForce.x) / (totalInverseMass + angularEffect.x);

		XMVECTOR fullImpulse = XMLoadFloat3(&cInfo.contactPoint.collisionNormal) * j;

		// Apply impulses
		rbA->ApplyLinearImpulse(-fullImpulse);
		rbB->ApplyLinearImpulse(fullImpulse);

		rbA->ApplyAngularImpulse(XMVector3Cross(relativeA, -fullImpulse));
		rbB->ApplyAngularImpulse(XMVector3Cross(relativeB, fullImpulse));
	}

	void PhysicsSystem::ApplyLinearTransformations(CCE::ECS::Components::Rigidbody* rbA, 
		CCE::ECS::Components::Transform* tfA, CCE::ECS::Components::Rigidbody* rbB, 
		CCE::ECS::Components::Transform* tfB, CCE::Physics::CollisionInfo& cInfo, float totalInverseMass) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		// Separating objects linearly
		XMFLOAT3 translationFirst{};
		XMFLOAT3 translationSecond{};

		XMStoreFloat3(&translationFirst, XMLoadFloat3(&tfA->Position()) - (XMVectorScale(XMLoadFloat3(&cInfo.contactPoint.collisionNormal),
			cInfo.contactPoint.penetration * (rbA->InverseMass() / totalInverseMass))));

		XMStoreFloat3(&translationSecond, XMLoadFloat3(&tfB->Position()) + (XMVectorScale(XMLoadFloat3(&cInfo.contactPoint.collisionNormal),
			cInfo.contactPoint.penetration * (rbB->InverseMass() / totalInverseMass))));

		tfA->SetTranslation(translationFirst);
		tfB->SetTranslation(translationSecond);
	}

	std::vector<long long> PhysicsSystem::PhysicsWorld{};
	double PhysicsSystem::PhysicsCalcDuration = 0.0;
	bool PhysicsSystem::applyAngularImpulse = false;
}
