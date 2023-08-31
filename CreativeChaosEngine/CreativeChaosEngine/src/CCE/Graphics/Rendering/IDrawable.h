#pragma once
#include "../../Core.h"
#include  "../RenderPipeline.h"

#include <memory>

namespace CCE::Graphics
{
	class IBindable;
	class IndexBuffer;

	struct CCE_API IDrawable
	{
	public:
		IDrawable() = default;
		virtual ~IDrawable() = default;
		void Draw() const noexcept;

	protected:
		void AddBind(std::shared_ptr<IBindable> bind) noexcept;

	private:
		const IndexBuffer* pIndexBuffer = nullptr;
		std::vector<std::shared_ptr<IBindable>> binds;
	};
}
