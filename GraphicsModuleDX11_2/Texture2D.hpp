#pragma once
#include "framework.h"
#include "interface.h"
class Texture2D : public ITexture2D
{
public:
	class Builder;
protected:
	Texture2D();
public:
	Texture2D(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc);
protected:
	void InitializeViews(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc);
public:
	HRESULT STDMETHODCALLTYPE GetResource(ID3D11Texture2D** out);
	HRESULT STDMETHODCALLTYPE GetSize(Size2D<u32>* out);
	HRESULT STDMETHODCALLTYPE GetView(GUID, ID3D11View** out);
	DXGI_FORMAT STDMETHODCALLTYPE GetFormat();
	HRESULT STDMETHODCALLTYPE CreateGenerateMipmap(ID3D11Device* device, ID3D11Device* deviceContext);
protected:
	ComPtr<ID3D11Texture2D> texture;
	Size2D<u32> size;
	DXGI_FORMAT format;
	std::unordered_map<GUIDEx, ComPtr<ID3D11View> > views;
};

class Texture2D::Builder :public D3D11_TEXTURE2D_DESC
{
public:
	Builder(DXGI_FORMAT format, uint32_t width, uint32_t height);
	auto GenerateMips()->Builder&;
	auto UnorderedResourceView()->Builder&;
	auto DepthStancilView()->Builder&;
	auto Build(ID3D11Device* device)->Texture2D*;
};