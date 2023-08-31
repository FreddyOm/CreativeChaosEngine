#include "IDrawable.h"
#include "IndexBuffer.h"

namespace CCE::Graphics
{
	void IDrawable::Draw() const noexcept
	{
		for (auto& b : binds)
		{
			b->Bind();
		}
		RenderPipeline::Instance->GetDeviceContextPtr()->DrawIndexed(pIndexBuffer->GetCount(), 0u, 0u);
	}
	void IDrawable::AddBind(std::shared_ptr<IBindable> bind) noexcept
	{
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			DASSERT(pIndexBuffer == nullptr, "Binding multiple index buffers is not allowed!");
			pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
		}
		binds.push_back(std::move(bind));
	}
}

