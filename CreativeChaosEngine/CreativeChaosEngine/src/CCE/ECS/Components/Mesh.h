#pragma once
#include <memory>
#include "../../String/String.h"
#include "../../Resources/MeshData.h"
#include "../../Graphics/Rendering/Bindable/BindableCommon.h"
#include "../../Graphics/Rendering/D3D11.h"


namespace CCE::ECS::Components
{
	template<typename C>
	class ConstantBuffer;
	template<typename C>
	class VSConstantBuffer;

	class Mesh
	{
	public:
		Mesh() {};
		Mesh(String path);
		~Mesh();

		String Path() const;
		std::shared_ptr<Resources::MeshData> Data() const;

		void StaticBind();
		void DynamicBind(DirectX::XMMATRIX modelMatrix);

		void AddBind(const std::shared_ptr<Graphics::IBindable> bind) noexcept;
		void CreateConstBufs(const DirectX::XMMATRIX& modelMatrix);

	public:
		std::shared_ptr<Graphics::VSConstantBuffer<DirectX::XMFLOAT4X4>> pMeshConstBuf = nullptr;
		const Graphics::IndexBuffer* pIndexBuffer = nullptr;

	private:
		String meshPath = "";
		std::shared_ptr<Resources::MeshData> meshData{};
		std::vector<std::shared_ptr<Graphics::IBindable>> meshBindPtr = {};
	};
}
