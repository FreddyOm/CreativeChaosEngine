#include "IBindable.h"
#include "../../RenderPipeline.h"

namespace CCE::Graphics
{
	ID3D11DeviceContext* IBindable::GetContext() noexcept
	{
		OPTICK_EVENT();
		return RenderPipeline::Instance->GetDeviceContextPtr();
	}

	ID3D11Device* IBindable::GetDevice() noexcept
	{
		OPTICK_EVENT();
		return RenderPipeline::Instance->GetDevicePtr();
	}
}