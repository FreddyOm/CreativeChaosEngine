#include "IDrawable.h"
#include "../bindable/index-buffer.h"
#include "../../rendering.h"

namespace CCE::Graphics
{
	void IDrawable::Draw() const noexcept
	{
		OPTICK_EVENT();
		for (auto& b : binds)
		{
			b->DynamicBind();
		}
		g_pContext->DrawIndexed(pIndexBuffer->GetCount(), 0u, 0u);
	}

	void IDrawable::AddBind(std::shared_ptr<IBindable> bind) noexcept
	{
		OPTICK_EVENT();
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			DASSERT(pIndexBuffer == nullptr, "Binding multiple index buffers is not allowed!");
			pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
		}
		binds.push_back(std::move(bind));
	}
}

