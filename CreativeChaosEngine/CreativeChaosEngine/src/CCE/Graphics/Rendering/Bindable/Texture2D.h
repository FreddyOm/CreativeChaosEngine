#pragma once
#include "IBindable.h"
#include "../../../string/string.h"
#include "../../../multithreading/job-system.h"
#include <vector>
#include <wrl.h>

namespace CCE::Graphics
{
	struct Texture2D : public IBindable
	{
	public:
		Texture2D(String& filePath, unsigned short startSlot = 0);
		Texture2D(std::vector<String>& filePaths, unsigned short startSlot = 0);
		~Texture2D();

		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

	private:
		unsigned short count = 0;
		unsigned short startSlot = 0;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D = {};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceView = {};
		std::vector<D3D11_SUBRESOURCE_DATA> subresourceDataArr = {};
	};
}
