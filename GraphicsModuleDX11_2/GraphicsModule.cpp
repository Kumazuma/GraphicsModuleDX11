#include "pch.h"
#include "GraphicsModule.hpp"
#include <d3dcompiler.h>
#include <array>
#include "MeshCollection.h"
#include "SwapChain.hpp"
#include "AnimationSetCollection.h"
GraphicsModule::GraphicsModule()
{
    UINT creationFlags{ D3D11_CREATE_DEVICE_BGRA_SUPPORT };
    D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };

#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &device,
        nullptr,
        &deviceContext
    );
    CreateDXGIFactory(__uuidof(IDXGIFactory), &dxgiFactory);
//
	HRESULT hr;
	hr= ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_ALL, IID_IWICImagingFactory, (void**)&factory_);


	InitStaticMeshVertexShaderAndIA();
	InitSkinnedMeshVertexShaderAndIA();
}

void GraphicsModule::InitStaticMeshVertexShaderAndIA()
{
	HRESULT hr{};

	ComPtr<ID3DBlob> bytesCode;
	ComPtr<ID3DBlob> errMsg;
	hr = D3DCompileFromFile(L"./StaticMeshVS.hlsl", nullptr, nullptr, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &bytesCode, &errMsg);
	if (errMsg != nullptr)
	{
		OutputDebugStringA((char* const)errMsg->GetBufferPointer());
		return;
	}

	std::array< D3D11_INPUT_ELEMENT_DESC, 3> polygonLayout{};
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//float 3개가 들어간다는 약속
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;
	hr = device->CreateInputLayout(
		polygonLayout.data(),
		static_cast<UINT>(polygonLayout.size()),
		bytesCode->GetBufferPointer(),
		bytesCode->GetBufferSize(),
		&staticMeshInputLayout);
	device->CreateVertexShader(bytesCode->GetBufferPointer(),
		bytesCode->GetBufferSize(), nullptr, &staticMeshVertexShader);
}

void GraphicsModule::InitSkinnedMeshVertexShaderAndIA()
{
	HRESULT hr{};
	ComPtr<ID3DBlob> bytesCode;
	ComPtr<ID3DBlob> errMsg;
	hr = D3DCompileFromFile(L"./SkinnedMeshVS.hlsl", nullptr, nullptr, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &bytesCode, &errMsg);
	if (errMsg != nullptr)
	{
		OutputDebugStringA((char* const)errMsg->GetBufferPointer());
	}

	std::array< D3D11_INPUT_ELEMENT_DESC, 5> polygonLayout{};
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//float 3개가 들어간다는 약속
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;



	polygonLayout[3].SemanticName = "BLENDWEIGHT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BLENDINDICES";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32A32_UINT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;
	hr = device->CreateInputLayout(
		polygonLayout.data(),
		static_cast<UINT>(polygonLayout.size()),
		bytesCode->GetBufferPointer(),
		bytesCode->GetBufferSize(),
		&skinnedMeshInputLayout);
	device->CreateVertexShader(bytesCode->GetBufferPointer(),
		bytesCode->GetBufferSize(), nullptr, &skinnedMeshVertexShader);
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::CreateCBuffer(wchar_t const* name, size_t bufferSize)
{
	auto findIt{ cbuffers.find(name) };
	if (findIt != cbuffers.end())
	{
		return E_INVALIDARG;
	}
	ComPtr<ID3D11Buffer> newBuffer;
	HRESULT hr;
	CD3D11_BUFFER_DESC bufferDesc{ static_cast<UINT>(bufferSize), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE };
	hr = device->CreateBuffer(&bufferDesc, nullptr, &newBuffer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	cbuffers.emplace(name, newBuffer);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::GetDevice(ID3D11Device** out)
{
	if (out == nullptr)
	{
		return E_POINTER;
	}
	ID3D11Device* device = this->device.Get();
	*out = device;
	device->AddRef();
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::GetImmediateContext(ID3D11DeviceContext** out)
{
	if (out == nullptr)
	{
		return E_POINTER;
	}
	ID3D11DeviceContext* context = this->deviceContext.Get();
	*out = context;
	context->AddRef();
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::GetCBuffer(wchar_t const* name, ID3D11Buffer** out)
{
	auto findIt{ cbuffers.find(name) };
	if (findIt == cbuffers.end())
	{
		return E_INVALIDARG;
	}
	ID3D11Buffer* buffer = findIt->second.Get();
	*out = buffer;
	buffer->AddRef();
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::LoadPixelShader(wchar_t const* id, wchar_t const* path, char const* entry)
{
	HRESULT hr{};
	std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
		shaders[__uuidof(ID3D11PixelShader)];
	auto shaderIt{ pixelShaders.find(id) };
	if (shaderIt != pixelShaders.end())
	{
		return E_INVALIDARG;
	}
	ComPtr<ID3DBlob> code;
	ComPtr<ID3DBlob> errMsg;
	ComPtr<ID3D11PixelShader> pixelShader;
	hr = D3DCompileFromFile(path, nullptr, nullptr, entry, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code, &errMsg);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &pixelShader);
	pixelShaders.emplace(id, pixelShader);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::LoadPixelShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len)
{
	HRESULT hr{};
	std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
		shaders[__uuidof(ID3D11PixelShader)];
	auto shaderIt{ pixelShaders.find(id) };
	if (shaderIt != pixelShaders.end())
	{
		return E_INVALIDARG;
	}
	ComPtr<ID3DBlob> code;
	ComPtr<ID3DBlob> errMsg;
	ComPtr<ID3D11PixelShader> pixelShader;

	device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &pixelShader);
	pixelShaders.emplace(id, pixelShader);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::GetPixelShader(wchar_t const* id, ID3D11PixelShader** out)
{
	std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
		shaders[__uuidof(ID3D11PixelShader)];

	auto shaderIt{ pixelShaders.find(id) };
	if (shaderIt == pixelShaders.end())
	{
		return E_INVALIDARG;
	}
	shaderIt->second->QueryInterface(out);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::LoadComputeShader(wchar_t const* id, wchar_t const* path, char const* entry)
{
	HRESULT hr{};
	std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& computeShaders =
		shaders[__uuidof(ID3D11ComputeShader)];
	auto shaderIt{ computeShaders.find(id) };
	if (shaderIt != computeShaders.end())
	{
		return E_INVALIDARG;
	}
	ComPtr<ID3DBlob> code;
	ComPtr<ID3DBlob> errMsg;
	ComPtr<ID3D11ComputeShader> computeShader;
	hr = D3DCompileFromFile(path, nullptr, nullptr, entry, "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG
		, 0, &code, &errMsg);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	device->CreateComputeShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, &computeShader);
	computeShaders.emplace(id, computeShader);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::LoadComputeShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len)
{
	HRESULT hr{};
	std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& computeShaders =
		shaders[__uuidof(ID3D11ComputeShader)];
	auto shaderIt{ computeShaders.find(id) };
	if (shaderIt != computeShaders.end())
	{
		return E_INVALIDARG;
	}
	ComPtr<ID3D11ComputeShader> computeShader;
	device->CreateComputeShader(ptr, len, nullptr, &computeShader);
	computeShaders.emplace(id, computeShader);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::GetComputeShader(wchar_t const* id, ID3D11ComputeShader** out)
{
	std::unordered_map<std::wstring, ComPtr<ID3D11DeviceChild> >& pixelShaders =
		shaders[__uuidof(ID3D11ComputeShader)];

	auto shaderIt{ pixelShaders.find(id) };
	if (shaderIt == pixelShaders.end())
	{
		return E_INVALIDARG;
	}
	shaderIt->second->QueryInterface(out);
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) GraphicsModule::GetVertexShaderAndIALayout(MeshType meshType, ID3D11VertexShader** ppOut, ID3D11InputLayout** ppLayout)
{
	if (ppOut == nullptr)
		return E_POINTER;
	if (ppLayout == nullptr)
		return E_POINTER;
	ID3D11VertexShader* vertexShader{};
	ID3D11InputLayout* inputLayout{};
	switch (meshType)
	{
	case MeshType::Static:
		vertexShader = staticMeshVertexShader.Get();
		inputLayout = staticMeshInputLayout.Get();
		break;
	case MeshType::Skinned:
		vertexShader = skinnedMeshVertexShader.Get();
		inputLayout =skinnedMeshInputLayout.Get();
	}
	*ppOut = vertexShader;
	*ppLayout = inputLayout;
	vertexShader->AddRef();
	inputLayout->AddRef();
    return S_OK;
}

HRESULT __stdcall GraphicsModule::CreateSwapChain(ISwapChain** out, HWND hWnd, Size2D<u32> const* bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen)
{
	if (out == nullptr) return E_POINTER;
	if (bufferSize == nullptr) return E_INVALIDARG;
	try
	{
		auto swapChain = new RefCountImpl<SwapChain, ISwapChain, IUnknown>(hWnd, this, *bufferSize, bufferFormat, fullScreen);
		*out = swapChain;
		return S_OK;
	}
	catch (...)
	{

	}
	return E_FAIL;
}

HRESULT __stdcall GraphicsModule::CreateTexture(ITexture2D** out, D3D11_TEXTURE2D_DESC const* desc)
{
	if (out == nullptr)return E_POINTER;
	if (desc == nullptr)return E_INVALIDARG;
	try
	{
		auto texture2D = new RefCountImpl<Texture2D, ITexture2D, IUnknown>(device.Get(), *desc);
		*out = texture2D;
	}
	catch (...)
	{

	}
	return E_FAIL;
}

HRESULT __stdcall GraphicsModule::LoadMesh(wchar_t const* path, IMeshCollection** out)
{
	if (out == nullptr) return E_POINTER;
	if (path == nullptr) return E_INVALIDARG;
	try
	{
		auto meshCollection = new RefCountImpl<MeshCollection, IMeshCollection, IUnknown>(this, path);
		*out = meshCollection;
		return S_OK;
	}
	catch (...)
	{

	}
	return E_FAIL;
}

HRESULT __stdcall GraphicsModule::LoadTexture(ITexture2D** out, wchar_t const* path)
{
	HRESULT hr{};
	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICBitmapFrameDecode> frameDecoder;
	ComPtr<IWICFormatConverter> fmtConverter;

	hr = factory_->CreateDecoderFromFilename(path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

	if (FAILED(hr))
	{
		return E_FAIL;
	}
	assert(hr == S_OK);
	hr = decoder->GetFrame(0, &frameDecoder);
	assert(hr == S_OK);
	hr = factory_->CreateFormatConverter(&fmtConverter);
	assert(hr == S_OK);
	hr = fmtConverter->Initialize(frameDecoder.Get(), GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
	UINT width{};
	UINT height{};
	fmtConverter->GetSize(&width, &height);
	std::vector<uint8_t> imageData(static_cast<size_t>(width) * static_cast<size_t>(height) * 4, 0);
	hr = fmtConverter->CopyPixels(nullptr, width * 4, static_cast<UINT>(imageData.size()), imageData.data());
	CD3D11_TEXTURE2D_DESC texture2DDesc =
		CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	texture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	Texture2D* texture2d{ new RefCountImpl<Texture2D, ITexture2D, IUnknown>(device.Get(), texture2DDesc) };
	ComPtr<ID3D11Texture2D> d3d11Texture;
	texture2d->GetResource(&d3d11Texture);
	deviceContext->UpdateSubresource(d3d11Texture.Get(), 0, nullptr, imageData.data(), static_cast<UINT>(imageData.size() / height), 0);
	deviceContext->GenerateMips(texture2d->GetViewRef<ID3D11ShaderResourceView>());

	*out = texture2d;
	return S_OK;
}

HRESULT __stdcall GraphicsModule::LoadAnimationSet(IAnimationSetCollection** out, wchar_t const* path)
{
	if (out == nullptr)return E_POINTER;
	if (path == nullptr)return E_INVALIDARG;
	*out = new RefCountImpl<AnimationSetCollection, IAnimationSetCollection, IUnknown>(path);
	return S_OK;
}

IDXGIFactory* GraphicsModule::GetDXGIFactory()
{
	return dxgiFactory.Get();
}
