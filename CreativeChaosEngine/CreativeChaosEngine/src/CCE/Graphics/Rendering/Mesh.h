#pragma once
#include "IDrawable.h"
#include "../../Core.h"
#include "D3D11.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"

namespace CCE::Graphics
{
	struct CCE_API Mesh : public IDrawable
	{
	public:
		Mesh();
		~Mesh();

		// From IDrawable
		void DrawIndexed(UINT count) override;

		PixelShader ps;
		VertexShader vs;

	private:
		VertexBuffer* vertexBuf = nullptr;
		IndexBuffer* indexBuf = nullptr;
		// TODO: Somehow implement a GUID for identification
	};
}
