#pragma once
#include "framework.h"
#include "interface.h"
#include "Texture2D.hpp"
class SwapChain : public ISwapChain
{
public:
	SwapChain(HWND hWindow, IGraphicsModule* gModule, Size2D<u32> const& bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen);
	HRESULT __stdcall GetBackBuffer(ITexture2D** out);
	HRESULT __stdcall GetDepthBuffer(ITexture2D** out);
	HRESULT __stdcall Clear(ID3D11DeviceContext* context, float const rgba[4], float depth);
	HRESULT __stdcall Present();
private:
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<Texture2D> backbuffer;
	ComPtr<Texture2D> depthBuffer;
};