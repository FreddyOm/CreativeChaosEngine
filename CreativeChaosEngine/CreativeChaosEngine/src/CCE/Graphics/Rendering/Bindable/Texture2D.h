#pragma once
#include "IBindable.h"
#include "../../../String/String.h"
#include <vector>
#include <wrl.h>

namespace CCE::Graphics
{
	struct CCE_API Texture2D : public IBindable
	{
	public:
		Texture2D(String& filePath);
		Texture2D(std::vector<String>& filePath);
		~Texture2D();

		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

	private:
		unsigned short count = 0;
		std::vector<String> resourcePaths = {};
		std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>> pTexture2DArray = {};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pResourceView = {};
	};
}
