#include "Texture2D.h"
#include "../../../Utilities/FreeImg/FreeImage.h"
#include "../../../Resources/TextureLoader.h"

namespace CCE::Graphics
{
	Texture2D::Texture2D(String& filePath)
	{
		count = 1;
		CCE::Resources::TextureLoader loader;
		auto tex = loader.LoadResource(filePath);

		FIBITMAP* texture = &tex->bitmap;

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = tex->width;
		texDesc.Height = tex->height;
		texDesc.ArraySize = count;
		texDesc.MipLevels = 1;
		texDesc.CPUAccessFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.MiscFlags = 0;
		switch (tex->format)
		{
		case FIF_PNG:
			texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			break;
		case FIF_JPEG:
			// 24bit images not allowed in D3D11! Convert to 32 bit!
			texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			texture = FreeImage_ConvertTo32Bits(&tex->bitmap);
			//FreeImage_ConvertTo32Bits(&tex->bitmap);
			break;
		defaut:
			DERROR("Unknown file format!");
			texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			break;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = texDesc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = texDesc.MipLevels;
		srvd.Texture2D.MostDetailedMip = 0;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = FreeImage_GetBits(texture);
		srd.SysMemPitch = (32 / 8) * tex->width;

		pTexture2DArray.push_back({});		// Push back empty objects in order to reference them via pp		
		GetDevice()->CreateTexture2D(&texDesc, &srd, pTexture2DArray.at(0).GetAddressOf());

		DASSERT(GetDevice()->CreateShaderResourceView(pTexture2DArray.at(0).Get(), &srvd, pResourceView.GetAddressOf()) == S_OK,
			"Failed creating the shader resource view!");
	}

	Texture2D::Texture2D(std::vector<String>& filePaths)
	{
		count = filePaths.size();
		resourcePaths = filePaths;
		{
			// Loads the texture for every given path and create the texture array accordingly
			for (unsigned short i = 0; i < count; ++i)
			{
				CCE::Resources::TextureLoader loader;
				auto tex = loader.LoadResource(filePaths.at(i));

				FIBITMAP* texture = &tex->bitmap;

				D3D11_TEXTURE2D_DESC texDesc = {};
				texDesc.Width = tex->width;
				texDesc.Height = tex->height;
				texDesc.MiscFlags = 0;
				texDesc.ArraySize = count;
				texDesc.MipLevels = 1;
				texDesc.CPUAccessFlags = 0;
				texDesc.SampleDesc.Count = 1;
				texDesc.SampleDesc.Quality = 0;
				texDesc.Usage = D3D11_USAGE_IMMUTABLE;
				texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				switch (tex->format)
				{
				case FIF_PNG:
					texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
					break;
				case FIF_JPEG:
					// 24bit images not allowed in D3D11! Convert to 32 bit!
					texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
					texture = FreeImage_ConvertTo32Bits(&tex->bitmap);
					//FreeImage_Unload(&tex->bitmap);
					break;
				defaut:
					DERROR("Unknown file format!");
					texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
					break;
				}

				D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
				srvd.Format = texDesc.Format;
				srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvd.Texture2D.MipLevels = texDesc.MipLevels;
				srvd.Texture2D.MostDetailedMip = 0;

				D3D11_SUBRESOURCE_DATA srd = {};
				srd.pSysMem = FreeImage_GetBits(texture);
				srd.SysMemPitch = (32 / 8) * tex->width;

				pTexture2DArray.push_back({});		// Push back empty objects in order to reference them via pp		
				GetDevice()->CreateTexture2D(&texDesc, &srd, pTexture2DArray.at(i).GetAddressOf());

				DASSERT(GetDevice()->CreateShaderResourceView(pTexture2DArray.at(i).Get(), &srvd, pResourceView.GetAddressOf()) == S_OK,
					"Failed creating the shader resource view!");
			}
		}
	}

	Texture2D::~Texture2D()
	{
		resourcePaths.clear();
		pResourceView.Reset();

		for (auto& t : pTexture2DArray)
			t.Reset();

		pTexture2DArray.clear();
	}

	/// <summary>
	/// Dynamically binding resources i.e. binding resources every frame.
	/// </summary>
	void Texture2D::DynamicBind()
	{
		// Bind to pipeline
		GetContext()->PSSetShaderResources(0, count, pResourceView.GetAddressOf());
	}

	/// <summary>
	/// Statically binding resources i.e. binding resources which only have to be bound once.
	/// </summary>
	void Texture2D::StaticBind()
	{
		
	}
}