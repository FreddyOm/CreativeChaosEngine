#pragma once
#include "IBindable.h"
#include "../D3D11.h"

namespace CCE::Graphics
{
	template<typename C>
	struct CCE_API ConstantBuffer : public IBindable
	{
	public:
		ConstantBuffer(const C& _constant, UINT _slot = 0u)
			: slot(_slot)
		{
			D3D11_BUFFER_DESC constBufDescription;
			constBufDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constBufDescription.Usage = D3D11_USAGE_DYNAMIC;
			constBufDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBufDescription.MiscFlags = 0u;
			constBufDescription.ByteWidth = sizeof(_constant);
			constBufDescription.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA subresourceData = {};
			subresourceData.pSysMem = &_constant;

			RenderPipeline::Instance->GetDevicePtr()->CreateBuffer(
				&constBufDescription, &subresourceData, &pConstantBuffer);
		}

		virtual void UpdateConstantBuffer(const C& _constant)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			RenderPipeline::Instance->GetDeviceContextPtr()->Map(
				pConstantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, &_constant, sizeof(C));
			RenderPipeline::Instance->GetDeviceContextPtr()->Unmap(
				pConstantBuffer.Get(), 0);
		}

	protected:
		UINT slot;
		UINT resourceCount;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	};
	
	template<typename C>
	struct CCE_API PSConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::slot;
		using ConstantBuffer<C>::pConstantBuffer;
	public:
		template<typename C>
		PSConstantBuffer(const C& consts, UINT slot, UINT _resourceCount = 1u)
			: ConstantBuffer<C>(consts, slot)
		{ }

		// Inherited by IBindable
		void Bind() override
		{
			RenderPipeline::Instance->GetDeviceContextPtr()->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};
	
	template<typename C>
	struct CCE_API VSConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::slot;
		using ConstantBuffer<C>::pConstantBuffer;
	public:
		template<typename C>
		VSConstantBuffer(const C& consts, UINT slot)
			: ConstantBuffer<C>(consts, slot)
		{ }

		// Inherited by IBindable
		void Bind() override
		{
			 RenderPipeline::Instance->GetDeviceContextPtr()->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
		}
	};
}
