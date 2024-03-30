#include "model.h"
#include "../../rendering.h"
#include "../resources/mesh-loader.h"
#include "../bindable/bindable-common.h"
#include "../../../manager/application.h"
#include <memory>

namespace CCE::Graphics
{
	Model::Model(String path)
	{
		OPTICK_EVENT();
		CCE::Resources::MeshLoader meshLoader;
		std::shared_ptr<Resources::MeshData> meshData = meshLoader.LoadResource(path);

		std::vector<std::shared_ptr<IBindable>> bindPtrs;

		String pixelShaderPath = Application::Instance->resourceDataPath.Path() + "/shader/DefaultPixelShader.cso";
		String vertexShader = Application::Instance->resourceDataPath.Path() + "/shader/DefaultVertexShader.cso";
		String diffuseTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_albedo.jpeg";
		String normalTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_normal.jpeg";

		std::vector<String> texturePaths = { normalTexFilePath, diffuseTexFilePath };


		// Material
		auto ps = std::make_shared<PixelShader>(StringConverter::StringToWString(pixelShaderPath.Value()));

		auto albedoTex = std::make_shared<Texture2D>(diffuseTexFilePath);
		auto normalTex = std::make_shared<Texture2D>(normalTexFilePath, 1);
		auto splr = std::make_shared<Sampler>(D3D11_TEXTURE_ADDRESS_WRAP);

		// Mesh
		auto ib = std::make_shared<IndexBuffer>(meshData->IndexBuffer);
		auto vb = std::make_shared<VertexBuffer>(meshData->VertexData);
		auto vs = std::make_shared<VertexShader>(StringConverter::StringToWString(vertexShader.Value()));
		auto il = std::make_shared<InputLayout>(vs->GetBytecode());
		auto to = std::make_shared<Topology>();

		AddBind(std::move(albedoTex));
		AddBind(std::move(normalTex));
		AddBind(std::move(splr));
		AddBind(std::move(to));
		AddBind(std::move(ib));
		AddBind(std::move(vb));
		AddBind(std::move(ps));
		AddBind(std::move(vs));
		AddBind(std::move(il));

		CreateConstBufs();

		transform.SetTranslation({ 0.0, 0.0, 0.0 });
		transform.SetScale({1, 1, 1});

		REGISTER_LEAK_DETECT;
	}

	/// <summary>
	/// Draw the mesh.
	/// </summary>
	void Model::Draw()
	{
		OPTICK_EVENT();
		IDrawable::Draw();

		// @TODO: Only do this when necessary
		XMStoreFloat4x4(&modelMatrix, transform.GetTransformationMatrix());

		pMeshConstBuf->UpdateConstantBuffer(modelMatrix);
		pMeshConstBuf->DynamicBind();
	}

	void Model::CreateConstBufs()
	{
		OPTICK_EVENT();
		XMStoreFloat4x4(&modelMatrix, transform.GetTransformationMatrix());
		pMeshConstBuf = std::make_shared<VSConstantBuffer<DirectX::XMFLOAT4X4>>(modelMatrix, 0);
	}
}
