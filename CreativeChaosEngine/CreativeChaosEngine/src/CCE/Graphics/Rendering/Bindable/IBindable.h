#pragma once
#include "../../../Core.h"
#include "../D3D11.h"
#include "../../Thirdparty/src/optick.h"

namespace CCE::Graphics
{
	class RenderPipeline;

	struct IBindable
	{
	public:
		IBindable() = default;
		virtual ~IBindable() = default;
		virtual void DynamicBind() = 0;
		virtual void StaticBind() = 0;

	protected:
		ID3D11DeviceContext* GetContext() noexcept;
		ID3D11Device* GetDevice() noexcept;
	};
}
