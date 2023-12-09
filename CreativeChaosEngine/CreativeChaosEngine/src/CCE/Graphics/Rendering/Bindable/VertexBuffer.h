#pragma once
#include "IBindable.h"
#include "../../../Core.h"
#include "../Vertex.h"
#include <vector>

namespace CCE::Graphics
{
	struct CCE_API VertexBuffer : public IBindable
	{
	public:
		VertexBuffer(std::vector<Vertex>& vertexBuffer);
		~VertexBuffer();

		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

	private:
		UINT count;
		ComPtr<ID3D11Buffer> pVertexBuffer;		
	};
}
