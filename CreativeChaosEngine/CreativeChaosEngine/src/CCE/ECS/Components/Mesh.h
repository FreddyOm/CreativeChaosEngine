#pragma once
#include <memory>
#include "../../string/string.h"
#include "../../resources/mesh-data.h"
#include "../../graphics/rendering/bindable/bindable-common.h"
#include "../../graphics/rendering/D3D11.h"
#include "../../graphics/rendering/bindable/vs-const-buf-data.h"


namespace CCE::ECS::Components
{
	template<typename C>
	class ConstantBuffer;
	template<typename C>
	class VSConstantBuffer;

	class Mesh
	{
	public:
		enum class RenderMode
		{
			Solid,
			Wireframe
		};

		Mesh() {};
		Mesh(String path);
		~Mesh();

		String Path() const;
		std::shared_ptr<Resources::MeshData> Data() const;

		void StaticBind();
		void DynamicBind(DirectX::XMMATRIX modelMatrix);

		void AddBind(const std::shared_ptr<Graphics::IBindable> bind) noexcept;
		void CreateConstBufs(const Graphics::VSConstBufData& constBufData);

	public:
		RenderMode RenderMode = RenderMode::Solid;
		std::shared_ptr<Graphics::VSConstantBuffer<Graphics::VSConstBufData>> pMeshConstBuf = nullptr;
		const Graphics::IndexBuffer* pIndexBuffer = nullptr;

	private:
		String meshPath = "";
		std::shared_ptr<Resources::MeshData> meshData{};
		std::vector<std::shared_ptr<Graphics::IBindable>> meshBindPtr = {};
	};
}
