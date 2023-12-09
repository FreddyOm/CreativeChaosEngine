#include "Mesh.h"
#include <memory>
#include "../../RenderPipeline.h"
#include "../Bindable/BindableCommon.h"
#include "../../../Manager/Application.h"

namespace CCE::Graphics
{
	Mesh::Mesh()
	{
		//std::vector<Vertex> vertices =
		//{
		//	{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(0, 0)},	// Left Upper Near
		//	{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0, 0) },	// Left Upper Far
		//	{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(0, 0) },		// Right Upper Far
		//	{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(0, 1) },	// Right Upper Near
		//	{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(1, 0) },  // Left Lower Near
		//	{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0, 0) },	// Left Lower Far
		//	{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(0, 0) },	// Right Lower Far
		//	{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1, 1) },	// Right Lower Near
		//};

		//std::vector<DWORD> indices =
		//{
		//	0,1,3, 1,2,3,	// top
		//	4,7,5, 5,7,6,	// bottom
		//	4,0,3, 4,3,7,	// front
		//	6,2,5, 5,2,1,	// back
		//	5,0,4, 5,1,0,	// left
		//	7,3,6, 6,3,2,	// right
		//};

		std::vector<Vertex> vertices =
		{
			{ XMFLOAT3(-1, 1, 0.0f), XMFLOAT2(0, 0)},		// Left Upper
			{ XMFLOAT3(1, 1, 0.0f), XMFLOAT2(1, 0) },		// Right Upper
			{ XMFLOAT3(-1, -1, 0.0f), XMFLOAT2(0, 1) },		// Left Lower
			{ XMFLOAT3(1, -1, 0.0f), XMFLOAT2(1, 1) },		// Right Lower
		};

		std::vector<DWORD> indices =
		{
			0,1,2, 2,1,3,	// front
		};

		std::vector<std::shared_ptr<IBindable>> bindPtrs;

		String pixelShaderPath = Application::Instance->resourceDataPath.Path() + "/shader/DefaultPixelShader.cso";
		String vertexShader = Application::Instance->resourceDataPath.Path() + "/shader/DefaultVertexShader.cso";
		String textureFilePath = Application::Instance->resourceDataPath.Path() + "/textures/test.png";

		std::string s = vertexShader.Value();

		auto tex = std::make_shared<Texture2D>(textureFilePath);
		auto splr = std::make_shared<Sampler>(D3D11_TEXTURE_ADDRESS_WRAP);
		auto ps = std::make_shared<PixelShader>(StringConverter::StringToWString(pixelShaderPath.Value()));
		auto vs = std::make_shared<VertexShader>(StringConverter::StringToWString(vertexShader.Value()));
		auto ib = std::make_shared<IndexBuffer>(indices);
		auto vb = std::make_shared<VertexBuffer>(vertices);
		auto il = std::make_shared<InputLayout>(vs->GetBytecode());
		auto to = std::make_shared<Topology>();

		bindPtrs.push_back(std::move(tex));
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
	void Mesh::Draw()
	{
		IDrawable::Draw();

		// TODO: Only do this when necessary
		XMStoreFloat4x4(&modelMatrix, transform.GetTransformationMatrix());

		pMeshConstBuf->UpdateConstantBuffer(modelMatrix);
		pMeshConstBuf->DynamicBind();
	}

	void Mesh::CreateConstBufs()
	{
		XMStoreFloat4x4(&modelMatrix, transform.GetTransformationMatrix());
		pMeshConstBuf = std::make_shared<VSConstantBuffer<DirectX::XMFLOAT4X4>>( modelMatrix, 0);
	}
}
