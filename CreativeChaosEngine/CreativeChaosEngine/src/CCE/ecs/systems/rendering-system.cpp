#include "rendering-system.h"
#include "../../graphics/rendering/bindable/vs-const-buf-data.h"
#include "../../graphics/rendering/bindable/constant-buffer.h"
#include "../components/component-headers.h"
#include "../entity-component-system.h"
#include "../../analysis/logger.h"
#include "../entity.h"
#include "../../graphics/rendering/D3D11/d3d11RenderPipeline.h"
#include "../manager/profilingManager.h"
#include "../multithreading/job-system.h"

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
		OPTICK_EVENT();
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		
		mEntities.insert(entity);
	}

	void RenderingSystem::UpdateSystem()
	{
		OPTICK_EVENT();
		UpdateECSBasic();
	}

	Jobs::JobReturnType UpdateEntities(uintptr_t id)
	{
		OPTICK_EVENT();
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;

		Entity e(static_cast<long long>(id));

		// @TODO: Check how to update each component buffer most effectively
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
		Graphics::g_pContext->DrawIndexed(mesh->pIndexBuffer->GetCount(), 0u, 0u);
	}

	void RenderingSystem::UpdateECSBasic()
	{
		OPTICK_EVENT();
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;

		Jobs::Counter cnt = Jobs::Counter(mEntities.size());

		for (long long id : mEntities)
		{
			//Jobs::Job updateEntitiesJob(UpdateEntities, &cnt, Jobs::Priority::NORMAL, static_cast<uintptr_t>(id));
			//Jobs::KickJob(std::move(updateEntitiesJob));
			UpdateEntities(id);
		}

		//Jobs::BusyWaitForCounter(&cnt);
	}
}
