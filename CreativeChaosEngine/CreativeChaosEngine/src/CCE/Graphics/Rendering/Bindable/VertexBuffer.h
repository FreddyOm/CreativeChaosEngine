#pragma once
#include "IBindable.h"
#include "../../../Core.h"
#include "../Vertex.h"
#include <vector>
#include <memory>

namespace CCE::Graphics
{
	struct VertexBuffer : public IBindable
	{
	public:
		VertexBuffer(std::shared_ptr<std::vector<Vertex>> vertexBuffer);
		~VertexBuffer();

		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

	private:
		UINT count;
		ComPtr<ID3D11Buffer> pVertexBuffer;		
	};
}
