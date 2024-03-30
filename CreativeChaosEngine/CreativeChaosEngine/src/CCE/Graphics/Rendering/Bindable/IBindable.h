#pragma once
#include "../../../core.h"
#include "../D3D11.h"
#include "../graphics/rendering.h"
#include "../../thirdparty/src/optick.h"

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
	};
}
