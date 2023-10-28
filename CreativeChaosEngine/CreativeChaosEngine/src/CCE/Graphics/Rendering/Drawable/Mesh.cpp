#include "Mesh.h"
#include "../Bindable/BindableCommon.h"
#include "../../RenderPipeline.h"
#include <memory>

namespace CCE::Graphics
{
	Mesh::Mesh(File _pixelShader, File _vertexShader)
		: pixelShader(_pixelShader), vertexShader(_vertexShader)
	{
		std::vector<Vertex> vertices =
		{
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f) },
			{ XMFLOAT3(0.5f, 0.5f, 0.5f) },
			{ XMFLOAT3(0.5f, 0.5f, -0.5f) },
			{ XMFLOAT3(-0.5f, -0.5f, -0.5f) },
			{ XMFLOAT3(-0.5f, -0.5f, 0.5f) },
			{ XMFLOAT3(0.5f, -0.5f, 0.5f) },
			{ XMFLOAT3(0.5f, -0.5f, -0.5f) },
		};

		std::vector<DWORD> indices =
		{
			0,1,3, 1,2,3,	// top
			4,7,5, 5,7,6,	// bottom
			4,0,3, 4,3,7,	// front
			6,2,5, 5,2,1,	// back
			5,0,4, 5,1,0,	// left
			7,3,6, 6,3,2,	// right
		};

		std::vector<std::shared_ptr<IBindable>> bindPtrs;

		// D:/Repositories/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultPixelShader.cso
		// D:/Repositories/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultVertexShader.cso

		auto ps = std::make_shared<PixelShader>(pixelShader.GetPathWSTDString());
		auto vs = std::make_shared<VertexShader>(vertexShader.GetPathWSTDString());
		auto ib = std::make_shared<IndexBuffer>(indices);
		auto vb = std::make_shared<VertexBuffer>(vertices);
		auto il = std::make_shared<InputLayout>(vs->GetBytecode());
		auto to = std::make_shared<Topology>();

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
		transform.SetScale({1.0, 1.0, 0.25});

		REGISTER_LEAK_DETECT;
	}

	/// <summary>
	/// Draw the mesh.
	/// </summary>
	void Mesh::Draw()
	{
		IDrawable::Draw();

		// TODO: Only do this when necessary
		modelMatrix = transform.GetTransformationMatrix();

		pMeshConstBuf->UpdateConstantBuffer(modelMatrix);
		pMeshConstBuf->Bind();
	}

	void Mesh::CreateConstBufs()
	{
		modelMatrix = transform.GetTransformationMatrix();
		pMeshConstBuf = std::make_shared<VSConstantBuffer<DirectX::XMMATRIX>>(modelMatrix, 0);
	}
}
