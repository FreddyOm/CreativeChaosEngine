#include "Model.h"
#include "../../RenderPipeline.h"
#include "../Resources/MeshLoader.h"
#include "../Bindable/BindableCommon.h"
#include "../../../Manager/Application.h"
#include <memory>

namespace CCE::Graphics
{
	Model::Model(String path)
	{
		CCE::Resources::MeshLoader meshLoader;
		std::unique_ptr<CCE::Resources::MeshData> meshData = meshLoader.LoadResource(path);

		std::vector<std::shared_ptr<IBindable>> bindPtrs;

		String pixelShaderPath = Application::Instance->resourceDataPath.Path() + "/shader/DefaultPixelShader.cso";
		String vertexShader = Application::Instance->resourceDataPath.Path() + "/shader/DefaultVertexShader.cso";
		String diffuseTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_albedo.jpeg";
		String normalTexFilePath = Application::Instance->resourceDataPath.Path() + "/models/textures/DefaultMaterial_normal.jpeg";

		std::vector<String> texturePaths = { normalTexFilePath, diffuseTexFilePath };

		auto albedoTex = std::make_shared<Texture2D>(diffuseTexFilePath);
		auto normalTex = std::make_shared<Texture2D>(normalTexFilePath, 1);
		auto splr = std::make_shared<Sampler>(D3D11_TEXTURE_ADDRESS_WRAP);
		auto ps = std::make_shared<PixelShader>(StringConverter::StringToWString(pixelShaderPath.Value()));
		auto vs = std::make_shared<VertexShader>(StringConverter::StringToWString(vertexShader.Value()));
		auto ib = std::make_shared<IndexBuffer>(meshData->IndexBuffer);
		auto vb = std::make_shared<VertexBuffer>(meshData->VertexData);
		auto il = std::make_shared<InputLayout>(vs->GetBytecode());
		auto to = std::make_shared<Topology>();

		bindPtrs.push_back(std::move(albedoTex));
		bindPtrs.push_back(std::move(normalTex));
		bindPtrs.push_back(std::move(splr));
		bindPtrs.push_back(std::move(to));
		bindPtrs.push_back(std::move(ib));
		bindPtrs.push_back(std::move(vb));
		bindPtrs.push_back(std::move(ps));
		bindPtrs.push_back(std::move(vs));
		bindPtrs.push_back(std::move(il));

		// This is currently redundant but could later be used to assemble 
		// all components of the mesh and add the bindables.
		for (auto& bind : bindPtrs)
		{
			AddBind(std::move(bind));
		}

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
		IDrawable::Draw();

		// TODO: Only do this when necessary
		XMStoreFloat4x4(&modelMatrix, transform.GetTransformationMatrix());

		pMeshConstBuf->UpdateConstantBuffer(modelMatrix);
		pMeshConstBuf->DynamicBind();
	}

	void Model::CreateConstBufs()
	{
		XMStoreFloat4x4(&modelMatrix, transform.GetTransformationMatrix());
		pMeshConstBuf = std::make_shared<VSConstantBuffer<DirectX::XMFLOAT4X4>>(modelMatrix, 0);
	}
}
