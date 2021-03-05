#pragma once
#include "interface.h"
#include "framework.h"
#include <wincodec.h>
class GraphicsModule : public IGraphicsModule
{
public:
	GraphicsModule();
	void InitStaticMeshVertexShaderAndIA();
	void InitSkinnedMeshVertexShaderAndIA();
	HRESULT __stdcall CreateCBuffer(wchar_t const* name, size_t bufferSize);
	HRESULT __stdcall GetDevice(ID3D11Device** out);
	HRESULT __stdcall GetImmediateContext(ID3D11DeviceContext** out);
	HRESULT __stdcall GetCBuffer(wchar_t const* name, ID3D11Buffer** out);
	HRESULT __stdcall LoadPixelShader(wchar_t const* id, wchar_t const* path, char const* entry);
	HRESULT __stdcall LoadPixelShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len);
	HRESULT __stdcall GetPixelShader(wchar_t const* id, ID3D11PixelShader** out);
	HRESULT __stdcall LoadComputeShader(wchar_t const* id, wchar_t const* path, char const* entry);
	HRESULT __stdcall LoadComputeShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len);
	HRESULT __stdcall GetComputeShader(wchar_t const* id, ID3D11ComputeShader** out);
	HRESULT __stdcall GetVertexShaderAndIALayout(MeshType meshType, ID3D11VertexShader** ppOut, ID3D11InputLayout** ppLayout);
	HRESULT __stdcall CreateSwapChain(ISwapChain** out, HWND hWnd, Size2D<u32> const* bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen);
	HRESULT __stdcall CreateTexture(ITexture2D** out, D3D11_TEXTURE2D_DESC const* desc);
	HRESULT __stdcall LoadMesh(wchar_t const* path, IMeshCollection** out);
	HRESULT __stdcall LoadTexture(ITexture2D** out, wchar_t const* path);
	HRESULT __stdcall LoadAnimationSet(IAnimationSetCollection** out, wchar_t const* path);

	IDXGIFactory* GetDXGIFactory();

private:
	ComPtr<ID3D11Device>		device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGIFactory>		dxgiFactory;
	ComPtr<IWICImagingFactory> factory_;

	std::unordered_map < GUIDEx, std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> > >shaders;
	std::unordered_map<std::wstring, ComPtr<ID3D11Buffer> > cbuffers;
	//
	ComPtr<ID3D11InputLayout>		staticMeshInputLayout;
	ComPtr<ID3D11VertexShader>		staticMeshVertexShader;
	//
	ComPtr<ID3D11InputLayout>		skinnedMeshInputLayout;
	ComPtr<ID3D11VertexShader>		skinnedMeshVertexShader;
};