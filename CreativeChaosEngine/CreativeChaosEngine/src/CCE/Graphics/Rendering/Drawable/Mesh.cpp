#include "Mesh.h"
#include "../Bindable/BindableCommon.h"
#include <memory>
#include "../../RenderPipeline.h"

namespace CCE::Graphics
{
	Mesh::Mesh()
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

		auto ps = std::make_shared<PixelShader>(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultPixelShader.cso");
		auto vs = std::make_shared<VertexShader>(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultVertexShader.cso");
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

		for (auto& bind : bindPtrs)
		{
			AddBind(std::move(bind));
		}

		transform.SetPosition({0.0, 0.3, 0.1});

		CreateConstBufs();

		REGISTER_LEAK_DETECT;
	}

	/// <summary>
	/// Draw the mesh.
	/// </summary>
	/// <param name="rp"></param>
	void Mesh::Draw()
	{
		IDrawable::Draw();

		// TODO: Only do this when necessary
		modelMatrix = transform.GetTransformationMatrix();

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		RenderPipeline::Instance->GetDeviceContextPtr()->Map(pPerObjectConstBuf.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &modelMatrix, sizeof(XMMATRIX));
		RenderPipeline::Instance->GetDeviceContextPtr()->Unmap(pPerObjectConstBuf.Get(), 0);

		RenderPipeline::Instance->GetDeviceContextPtr()->VSSetConstantBuffers(1, 1, pPerObjectConstBuf.GetAddressOf());
	}

	void Mesh::CreateConstBufs()
	{
		modelMatrix = transform.GetTransformationMatrix();

		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(XMMATRIX);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		RenderPipeline::Instance->GetDevicePtr()->CreateBuffer(
			&desc, nullptr, &pPerObjectConstBuf);
	
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		RenderPipeline::Instance->GetDeviceContextPtr()->Map(pPerObjectConstBuf.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, &modelMatrix, sizeof(XMMATRIX));
		RenderPipeline::Instance->GetDeviceContextPtr()->Unmap(pPerObjectConstBuf.Get(), 0);
		
		RenderPipeline::Instance->GetDeviceContextPtr()->VSSetConstantBuffers(1, 1, pPerObjectConstBuf.GetAddressOf());
	}
}
