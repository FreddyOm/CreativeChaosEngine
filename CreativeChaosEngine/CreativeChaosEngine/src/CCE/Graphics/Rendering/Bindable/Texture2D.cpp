#include "Texture2D.h"
#include "../../../Utilities/FreeImg/FreeImage.h"
#include "../../../Resources/TextureLoader.h"
#include "../../../Analysis/Debug.h"

namespace CCE::Graphics
{
#pragma region job funcs declarations

	JOB_ENTRY_POINT D3D11LoadImageResource(CCE::Resources::TextureLoader& loader, ID3D11Device* device, std::vector<String> const& filePaths, 
		std::vector<D3D11_SUBRESOURCE_DATA>& subresourceDataArr, Microsoft::WRL::ComPtr<ID3D11Texture2D>& pTexture2D, 
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pResourceView, unsigned short count);

	JOB_ENTRY_POINT D3D11SetImage(std::unique_ptr<CCE::Resources::TexData>& tex, D3D11_TEXTURE2D_DESC& texDesc,
		FIBITMAP* texture, std::vector<D3D11_SUBRESOURCE_DATA>& subresourceDataArr, D3D11_SHADER_RESOURCE_VIEW_DESC& srvd);

#pragma endregion job funcs declarations


	Texture2D::Texture2D(String& filePath, unsigned short startSlot)
		: count(1), startSlot(startSlot)
	{
		subresourceDataArr.reserve(count);
		CCE::Resources::TextureLoader loader;

		D3D11LoadImageResource(loader, GetDevice(), {filePath}, subresourceDataArr, pTexture2D, pResourceView, count);
	}

	// @TODO: How can I load every image in parallel?
	Texture2D::Texture2D(std::vector<String>& filePaths, unsigned short startSlot)
		: count(filePaths.size()), startSlot(startSlot)
	{
		subresourceDataArr.reserve(count);
		CCE::Resources::TextureLoader loader;

		D3D11LoadImageResource(loader, GetDevice(), filePaths, subresourceDataArr, pTexture2D, pResourceView, count);
	}

	/// <summary>
	/// The destructor of Texture2D.
	/// </summary>
	Texture2D::~Texture2D()
	{
		pTexture2D.Reset();
		subresourceDataArr.clear();
	}

	/// <summary>
	/// Dynamically binding resources i.e. binding resources every frame.
	/// </summary>
	void Texture2D::DynamicBind()
	{
		// Bind to pipeline
		//for(unsigned short s = 0; s < count; ++s)
		GetContext()->PSSetShaderResources(startSlot, count, pResourceView.GetAddressOf());
	}

	/// <summary>
	/// Statically binding resources i.e. binding resources which only have to be bound once.
	/// </summary>
	void Texture2D::StaticBind()
	{
		
	}

	// @TODO: Kick more leaf jobs (parallelize creation of texDesc and srvd)
	JOB_ENTRY_POINT D3D11LoadImageResource(CCE::Resources::TextureLoader& loader, ID3D11Device* device, std::vector<String> const& filePaths,
		std::vector<D3D11_SUBRESOURCE_DATA>& subresourceDataArr, Microsoft::WRL::ComPtr<ID3D11Texture2D>& pTexture2D, 
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pResourceView, unsigned short count)
	{
		D3D11_TEXTURE2D_DESC texDesc = {};

		texDesc.ArraySize = count;
		texDesc.MipLevels = 1;
		texDesc.CPUAccessFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.MiscFlags = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = texDesc.MipLevels;
		srvd.Texture2D.MostDetailedMip = 0;

		// @TODO: Parallelize this for loop by scheduling every image as a job
		for (unsigned short i = 0; i < count; ++i)
		{
			std::unique_ptr<CCE::Resources::TexData> tex = loader.LoadResource(filePaths.at(i));
			FIBITMAP* texture = &tex->bitmap;

			DASSERT(texDesc.Width == 0 || texDesc.Width == tex->width && texDesc.Height == tex->height,
				"All textures in a texture array must have the same dimension!");

			texDesc.Width = tex->width;
			texDesc.Height = tex->height;

			D3D11SetImage(tex, texDesc, texture, subresourceDataArr, srvd);
		}

		// Create texture resource and validate
		HRESULT hr = device->CreateTexture2D(&texDesc, subresourceDataArr.data(), pTexture2D.GetAddressOf());
		DASSERT(SUCCEEDED(hr), "Failed creating the texture resource!");
		
		// Create shader resource view and validate
		hr = device->CreateShaderResourceView(pTexture2D.Get(), &srvd, pResourceView.GetAddressOf());
		DASSERT(SUCCEEDED(hr), "Failed creating the shader resource view!");
	}

	// Set the actual image data and 
	JOB_ENTRY_POINT D3D11SetImage(std::unique_ptr<CCE::Resources::TexData>& tex, D3D11_TEXTURE2D_DESC& texDesc, 
		FIBITMAP* texture, std::vector<D3D11_SUBRESOURCE_DATA>& subresourceDataArr, D3D11_SHADER_RESOURCE_VIEW_DESC& srvd)
	{
		// Analyze image format
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

		srvd.Format = texDesc.Format;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = std::move(FreeImage_GetBits(texture));
		srd.SysMemPitch = (32 / 8) * tex->width;

		subresourceDataArr.push_back(std::move(srd));
	}
}
