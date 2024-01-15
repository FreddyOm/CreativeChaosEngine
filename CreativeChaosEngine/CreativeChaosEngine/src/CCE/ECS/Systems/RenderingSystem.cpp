#include "RenderingSystem.h"
#include "../../Graphics/Rendering/Bindable/VSConstBufData.h"
#include "../../Graphics/Rendering/Bindable/ConstantBuffer.h"
#include "../Components/ComponentHeaders.h"
#include "../EntityComponentSystem.h"
#include "../../Analysis/Logger.h"
#include "../Entity.h"
#include "../../Graphics/RenderPipeline.h"

namespace CCE::ECS::Systems
{
	using ECS = EntityComponentSystem;

	void RenderingSystem::StartUp()
	{
		ECS::Instance->RegisterSystem<RenderingSystem>();
		LOGC("RenderingSystem initialized!", COLOR_BLUE);
	}

	void RenderingSystem::ShutDown()
	{
		LOGC("RenderingSystem uninitialized!", COLOR_BLUE);
	}

	void RenderingSystem::RegisterEntity(long long entity)
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		
		mEntities.insert(entity);
	}

	void RenderingSystem::UpdateSystem()
	{
		UpdateECSBasic();
	}

	void RenderingSystem::UpdateECSBasic()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;

		for (long long id : mEntities)
		{
			Entity e(id);
			// @TODO: Implement here!

			auto* transform = e.GetComponent<Transform>();
			auto* material = e.GetComponent<Material>();
			auto* mesh = e.GetComponent<Mesh>();

			// Draw and update everything
			DirectX::XMFLOAT4X4 modelMatrix;
			XMStoreFloat4x4(&modelMatrix, transform->GetTransformationMatrix());

			mesh->pMeshConstBuf->UpdateConstantBuffer(Graphics::VSConstBufData(modelMatrix, material->BaseColor));
			mesh->pMeshConstBuf->DynamicBind();

			// Bind and draw indexed
			mesh->DynamicBind(transform->GetTransformationMatrix());
			material->DynamicBind();
			Graphics::RenderPipeline::Instance->GetDeviceContextPtr()->DrawIndexed(mesh->pIndexBuffer->GetCount(), 0u, 0u);
		}
	}
}
