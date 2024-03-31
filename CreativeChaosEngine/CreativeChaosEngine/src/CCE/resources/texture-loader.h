#pragma once
#include <memory>
#include "../core.h"
#include "D3D11.h"
#include "resource-loader.h"
#include "../utilities/free-img/FreeImage.h"

namespace CCE::Resources
{
	struct CCE_API TexData
	{
		unsigned int width = 0;
		unsigned int height = 0;
		FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
		FIBITMAP bitmap = {};
	};

	struct CCE_API TextureLoader : protected ResourceLoader<TexData>
	{
		TextureLoader() = default;
		~TextureLoader() = default;

		virtual std::shared_ptr<TexData> LoadResource(String filePath) override;
	};
}
