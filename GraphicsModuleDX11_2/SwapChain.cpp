#include "pch.h"
#include "SwapChain.hpp"
#include "GraphicsModule.hpp"
#include "Texture2D.hpp"
#include "helperClasses.hpp"
class SwapChainTexture2D : public Texture2D
{
public:
	SwapChainTexture2D(ID3D11Device* device, IDXGISwapChain* swapChain)
	{
		HRESULT hr;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &texture);
		assert(SUCCEEDED(hr));
		if (FAILED(hr))
		{
			throw hr;
		}
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);
		InitializeViews(device, desc);
		size = Size2D<u32>{ desc.Width, desc.Height };
	}
};
SwapChain::SwapChain(HWND hWindow, IGraphicsModule* gmodule, Size2D<u32> const& bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = bufferSize.width;
	swapChainDesc.BufferDesc.Height = bufferSize.height;
	swapChainDesc.BufferDesc.Format = bufferFormat;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.OutputWindow = hWindow;
	swapChainDesc.Windowed = !fullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	ComPtr<ID3D11Device> device;
	gmodule->GetDevice(&device);
	IDXGIFactory* dxgiFactory{ static_cast<GraphicsModule*>(gmodule)->GetDXGIFactory() };
	dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, &swapChain);
	backbuffer = new RefCountImpl<SwapChainTexture2D, ITexture2D, IUnknown>{ device.Get(), swapChain.Get()};
	backbuffer->Release();
	depthBuffer = new RefCountImpl<Texture2D, ITexture2D, IUnknown>(device.Get(), Texture2D::Builder(DXGI_FORMAT_D32_FLOAT, bufferSize.width, bufferSize.height).DepthStancilView());
}

HRESULT __stdcall SwapChain::GetBackBuffer(ITexture2D** out)
{
	if (out == nullptr)
	{
		return E_POINTER;
	}
	ITexture2D* texture = backbuffer.Get();
	texture->AddRef();
	*out = texture;
	return S_OK;
}

HRESULT __stdcall SwapChain::GetDepthBuffer(ITexture2D** out)
{
	if (out == nullptr)
	{
		return E_POINTER;
	}
	ITexture2D* texture = depthBuffer.Get();
	texture->AddRef();
	*out = texture;
	return S_OK;
}

HRESULT __stdcall SwapChain::Clear(ID3D11DeviceContext* context, float const rgba[4], float depth)
{
	context->ClearRenderTargetView(backbuffer->GetViewRef<ID3D11RenderTargetView>(), rgba);
	context->ClearDepthStencilView(depthBuffer->GetViewRef<ID3D11DepthStencilView>(), D3D11_CLEAR_DEPTH, depth, 0);
	return S_OK;
}

HRESULT __stdcall SwapChain::Present()
{
    swapChain->Present(1, 0);
	return S_OK;
}
