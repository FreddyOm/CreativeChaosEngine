#pragma once
#include "IDrawable.h"
#include "IBindable.h"
#include "../../Core.h"
#include "Vertex.h"
#include <memory>

namespace CCE::Graphics
{
	struct CCE_API Mesh : public IDrawable
	{
	public:
		Mesh();
		~Mesh();

		// From IDrawable
		void DrawIndexed(UINT count) override;
		void AddBind(std::shared_ptr<IBindable> bindable) noexcept;

	private:
		std::vector<std::shared_ptr<IBindable>> binds;
		// TODO: Somehow implement a GUID for identification
	};
}
