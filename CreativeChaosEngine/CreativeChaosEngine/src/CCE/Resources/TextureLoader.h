#pragma once
#include <memory>
#include "../Core.h"
#include "D3D11.h"
#include "ResourceLoader.h"
#include "../Utilities/FreeImg/FreeImage.h"

namespace CCE::Resources
{
	struct CCE_API TexData
	{
		unsigned int width;
		unsigned int height;
		FREE_IMAGE_FORMAT format;
		FIBITMAP bitmap;
	};

	struct CCE_API TextureLoader : protected ResourceLoader<TexData>
	{
		TextureLoader() = default;
		~TextureLoader() = default;

		virtual std::unique_ptr<TexData> LoadResource(String filePath) override;
	};
}
