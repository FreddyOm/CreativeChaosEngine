#pragma once
#include "../../../Core.h"
#include <vector>
#include <memory>
#include "../../Thirdparty/src/optick.h"

namespace CCE::Graphics
{
	class RenderPipeline;
	struct IBindable;
	struct IndexBuffer;

	struct CCE_API IDrawable
	{
	public:
		IDrawable() = default;
		~IDrawable() = default;
		void Draw() const noexcept;

	protected:
		void AddBind(std::shared_ptr<IBindable> bind) noexcept;

	private:
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::shared_ptr<IBindable>> binds;
	};
}
