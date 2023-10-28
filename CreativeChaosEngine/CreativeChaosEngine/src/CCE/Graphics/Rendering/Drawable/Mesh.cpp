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
			{ XMFLOAT3(0.f, 0.25f, 0.0f) },
			{ XMFLOAT3(0.25f, -0.25f, 0.0f) },
			{ XMFLOAT3(-0.25f, -0.25f, 0.0f) },
		};

		std::vector<DWORD> indices =
		{
			0,1,2,
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
	}

	/// <summary>
	/// Draw the mesh.
	/// </summary>
	/// <param name="rp"></param>
	void Mesh::Draw()
	{
		IDrawable::Draw();
	}	
}
