#pragma once
#include "../../Core.h"
#include "../RenderPipeline.h"

namespace CCE::Graphics
{
	struct CCE_API IBindable
	{
	public:
		IBindable() = default;
		virtual ~IBindable() = default;
		virtual void Bind() = 0;
	protected:

		static ID3D11DeviceContext* GetContext(RenderPipeline* rp) noexcept
		{
			return rp->GetDeviceContextPtr();
		}

		static ID3D11Device* GetDevice(RenderPipeline* rp) noexcept 
		{
			return rp->GetDevicePtr();
		}
	};
}
