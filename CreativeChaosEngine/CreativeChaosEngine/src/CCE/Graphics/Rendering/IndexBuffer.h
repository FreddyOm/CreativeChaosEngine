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
		IndexBuffer(std::vector<DWORD>& indices);
		~IndexBuffer();
		UINT GetCount() const noexcept;

		// Geerbt über IBindable
		void Bind() override;

	private:
		UINT count;
		ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}
