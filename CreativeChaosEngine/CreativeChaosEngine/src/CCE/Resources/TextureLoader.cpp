#include "TextureLoader.h"
#include "../Utilities/IO/IO.h"
#include "../Analysis/Debug.h"

namespace CCE::Resources 
{
	std::unique_ptr<TexData> TextureLoader::LoadResource(String filePath)
	{
		TexData texData;
		BYTE* bytes = new BYTE[41943040];
		ZeroMemory(bytes, 41943040);

		DWORD fileSize = IO::ReadBytes(filePath, bytes);
		DASSERT(fileSize > 0, 
			"Failed reading the resource file!");

		FIMEMORY* mem = FreeImage_OpenMemory(bytes, fileSize);
		DASSERT(nullptr != mem, 
			"Couldn't read file stream!");

		auto format = FreeImage_GetFileTypeFromMemory(mem);

		DASSERT(format != FIF_UNKNOWN, 
			"Couldn't resolve the resources format!");

		auto data = FreeImage_LoadFromMemory(format, mem);
		FreeImage_CloseMemory(mem);

		DASSERT(FreeImage_FlipVertical(data), "Failed to flip image vertically!");

		texData.width = FreeImage_GetWidth(data);
		texData.height = FreeImage_GetHeight(data);
		texData.format = format;
		texData.bitmap = std::move(*data);
		
		delete[] bytes;
		
		// Potentially convert to correct data format R8G8B8 -> R8G8B8A8
		return std::make_unique<TexData>(std::move(texData));
	}
}
