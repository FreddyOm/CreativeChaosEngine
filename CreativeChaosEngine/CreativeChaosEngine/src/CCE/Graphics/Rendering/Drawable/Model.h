#pragma once
#include "../bindable/IBindable.h"
#include "../../../manager/ProfilingManager.h"
#include "../../../utilities/io/io.h"
#include "../../../core.h"
#include "../../../ecs/components/transform.h"    // <-- @TODO: Remove later and use ECS for any given entity
#include "../vertex.h"
#include "IDrawable.h"
#include <memory>

namespace CCE::Graphics
{
	template<typename C>
	class ConstantBuffer;
	template<typename C>
	class VSConstantBuffer;

	struct CCE_API Model : public IDrawable
	{
	public:
		Model(String path);
		~Model()
		{
			UNREGISTER_LEAK_DETECT;
		}

		// From IDrawable
		void Draw();

		// @TODO: Somehow implement a GUID for identification
		
	private:
		void CreateConstBufs();

	public:
		ECS::Components::Transform transform = {};

		String pixelShader = {};
		String vertexShader = {};

	private:
		
		DirectX::XMFLOAT4X4 modelMatrix;
		std::shared_ptr<VSConstantBuffer<DirectX::XMFLOAT4X4>> pMeshConstBuf = nullptr;
	};
}
