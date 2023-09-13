#pragma once
#include "../../../Core.h"
#include <vector>
#include <memory>

namespace CCE::Graphics
{
	class RenderPipeline;
	class IBindable;
	class IndexBuffer;

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
