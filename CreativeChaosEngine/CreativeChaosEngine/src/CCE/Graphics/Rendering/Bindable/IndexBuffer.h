#pragma once
#include "IBindable.h"
#include "../../../Core.h"
#include <vector>
#include <wrl.h>

namespace CCE::Graphics
{
	struct RenderPipeline;
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
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}
