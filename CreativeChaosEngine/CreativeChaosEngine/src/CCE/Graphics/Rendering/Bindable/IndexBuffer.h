#pragma once
#include "IBindable.h"
#include "../../../Core.h"
#include <vector>
#include <wrl.h>
#include <memory>

namespace CCE::Graphics
{
	struct RenderPipeline;
	struct IndexBuffer : public IBindable
	{
	public:
		IndexBuffer(std::shared_ptr<std::vector<unsigned int>> indices);
		~IndexBuffer();
		UINT GetCount() const noexcept;

		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

	private:
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}
