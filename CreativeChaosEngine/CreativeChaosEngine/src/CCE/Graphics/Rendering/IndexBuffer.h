#pragma once
#include "IBindable.h"
#include "../../Core.h"
#include <vector>
#include "D3D11.h"

namespace CCE::Graphics
{
	struct CCE_API IndexBuffer : public IBindable
	{
	public:
		IndexBuffer(std::vector<unsigned int>& indices);
		~IndexBuffer();

		// Geerbt über IBindable
		void Bind() override;

	private:
		UINT count;
		ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}
