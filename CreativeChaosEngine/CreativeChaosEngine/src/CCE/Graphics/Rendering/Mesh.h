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
		~Mesh() = default;

		// From IDrawable
		void Draw();

		// TODO: Somehow implement a GUID for identification
	};
}
