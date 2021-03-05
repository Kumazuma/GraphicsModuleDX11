#include "pch.h"
#include "Texture2D.hpp"
#include "helperClasses.hpp"
Texture2D::Texture2D()
{
}

Texture2D::Texture2D(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc)
{
    HRESULT hr{};
    size = Size2D<u32>{ desc.Width, desc.Height };

    hr = device->CreateTexture2D(&desc, nullptr, &texture);
    if (FAILED(hr)) throw hr;
    InitializeViews(device, desc);
}

void Texture2D::InitializeViews(ID3D11Device* device, D3D11_TEXTURE2D_DESC const& desc)
{
    format = desc.Format;
    HRESULT hr;
    if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
    {
        ComPtr<ID3D11ShaderResourceView> srv;

        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{ };
        shaderResourceViewDesc.Format = desc.Format;
        if (desc.ArraySize == 1)
        {
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = (desc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS) ? -1 : desc.MipLevels;
        }
        else
        {
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2DArray.MipLevels = (desc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS) ? -1 : desc.MipLevels;
            shaderResourceViewDesc.Texture2DArray.ArraySize = desc.ArraySize;
            shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
        }
        hr = device->CreateShaderResourceView(texture.Get(), &shaderResourceViewDesc, &srv);
        if (FAILED(hr)) throw hr;

        ComPtr<ID3D11View> view;
        srv.As(&view);
        views.emplace(__uuidof(ID3D11ShaderResourceView), view);
    }
    if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
    {
        ComPtr<ID3D11RenderTargetView> rtv;
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
        renderTargetViewDesc.Format = desc.Format;
        if (desc.ArraySize == 1)
        {
            renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            renderTargetViewDesc.Texture2D.MipSlice = 0;
        }
        else
        {
            renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            renderTargetViewDesc.Texture2DArray.ArraySize = desc.ArraySize;
            renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
            renderTargetViewDesc.Texture2DArray.MipSlice = 0;
        }
        hr = device->CreateRenderTargetView(texture.Get(), &renderTargetViewDesc, &rtv);
        if (FAILED(hr)) throw hr;

        ComPtr<ID3D11View> view;
        rtv.As(&view);
        views.emplace(__uuidof(ID3D11RenderTargetView), view);
    }
    if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
    {
        ComPtr<ID3D11UnorderedAccessView> uav;
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
        uavDesc.Format = desc.Format;
        if (desc.ArraySize == 1)
        {
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            uavDesc.Texture2D.MipSlice = 0;
        }
        else
        {
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
            uavDesc.Texture2DArray.ArraySize = desc.ArraySize;
            uavDesc.Texture2DArray.FirstArraySlice = 0;
            uavDesc.Texture2DArray.MipSlice = 0;
        }
        hr = device->CreateUnorderedAccessView(texture.Get(), &uavDesc, &uav);
        if (FAILED(hr)) throw hr;
        ComPtr<ID3D11View> view;
        uav.As(&view);
        views.emplace(__uuidof(ID3D11UnorderedAccessView), view);
    }
    if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        ComPtr<ID3D11DepthStencilView> depView;
        D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;
        desc.Format = desc.Format;
        hr = device->CreateDepthStencilView(texture.Get(), &desc, &depView);
        if (FAILED(hr)) throw hr;
        ComPtr<ID3D11View> view;
        depView.As(&view);
        views.emplace(__uuidof(ID3D11DepthStencilView), view);
    }
}

HRESULT __stdcall Texture2D::GetResource(ID3D11Texture2D** out)
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    ID3D11Texture2D* tex = this->texture.Get();
    *out = tex;
    tex->AddRef();
    return S_OK;
}

HRESULT __stdcall Texture2D::GetSize(Size2D<u32>* out)
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    *out = size;
    return S_OK;
}

HRESULT __stdcall Texture2D::GetView(GUID guid, ID3D11View** out)
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    auto findIt = views.find(guid);
    if (findIt == views.end())
    {
        return E_INVALIDARG;
    }
    ID3D11View* view = findIt->second.Get();
    *out = view;
    view->AddRef();
    return S_OK;
}

DXGI_FORMAT __stdcall Texture2D::GetFormat()
{
    return format;
}

HRESULT __stdcall Texture2D::CreateGenerateMipmap(ID3D11Device* device, ID3D11Device* deviceContext)
{
    return E_NOTIMPL;
}

Texture2D::Builder::Builder(DXGI_FORMAT format, uint32_t width, uint32_t height) :
    D3D11_TEXTURE2D_DESC{}
{
    this->ArraySize = 1;
    this->MipLevels = 1;
    this->Usage = D3D11_USAGE_DEFAULT;
    this->BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    this->Format = format;
    this->SampleDesc.Count = 1;
    this->SampleDesc.Quality = 0;
    this->Width = width;
    this->Height = height;
}

auto Texture2D::Builder::GenerateMips() -> Builder&
{
    this->MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
    this->MipLevels = -1;
    return *this;
}

auto Texture2D::Builder::UnorderedResourceView() -> Builder&
{
    this->BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
    return *this;
}

auto Texture2D::Builder::DepthStancilView() -> Builder&
{
    this->BindFlags = D3D11_BIND_DEPTH_STENCIL;
    this->ArraySize = 1;
    this->MipLevels = 1;
    this->Usage = D3D11_USAGE_DEFAULT;
    return *this;
}
auto Texture2D::Builder::Build(ID3D11Device* device) -> Texture2D*
{
    return new RefCountImpl<Texture2D, ITexture2D, IUnknown>(device, *this);
}