#pragma once
#include "IBindable.h"
#include "../../Core.h"
#include "D3D11.h"
#include "Vertex.h"
#include <vector>
#include "../RenderPipeline.h"

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
