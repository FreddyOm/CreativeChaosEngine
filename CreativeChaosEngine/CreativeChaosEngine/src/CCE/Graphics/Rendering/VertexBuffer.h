#pragma once
#include "IBindable.h"
#include "../../Core.h"
#include <vector>
#include "Vertex.h"

namespace CCE::Graphics
{
	struct CCE_API VertexBuffer : public IBindable
	{
	public:
		VertexBuffer(std::vector<Vertex>& vertexBuffer);
		~VertexBuffer();

		// Geerbt über IBindable
		void Bind() override;

	private:
		UINT count;
		ComPtr<ID3D11Buffer> pVertexBuffer;		
	};
}
