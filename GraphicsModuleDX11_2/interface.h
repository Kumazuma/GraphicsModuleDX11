#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include"typedef.h"
struct ITexture2D;
struct ISwapChain;
struct IMeshCollection;
struct IAnimationSet;
struct IAnimationSetCollection;
struct IBoneCollection;
MIDL_INTERFACE("C8DFC34E-0C36-42BF-BF38-6652CA1F41D1") IGraphicsModule : public IUnknown
{
	STDMETHOD(CreateCBuffer(wchar_t const* name, size_t bufferSize)) PURE;
	STDMETHOD(GetDevice(ID3D11Device** out)) PURE;
	STDMETHOD(GetImmediateContext(ID3D11DeviceContext** out)) PURE;
	STDMETHOD(GetCBuffer(wchar_t const* name, ID3D11Buffer** out)) PURE;
	STDMETHOD(LoadPixelShader(wchar_t const* id, wchar_t const* path, char const* entry)) PURE;
	STDMETHOD(LoadPixelShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len)) PURE;
	STDMETHOD(GetPixelShader(wchar_t const* id, ID3D11PixelShader** out)) PURE;
	STDMETHOD(LoadComputeShader(wchar_t const* id, wchar_t const* path, char const* entry)) PURE;
	STDMETHOD(LoadComputeShaderFromBytes(wchar_t const* id, u8 const* ptr, u32 len)) PURE;
	STDMETHOD(GetComputeShader(wchar_t const* id, ID3D11ComputeShader** out)) PURE;
	STDMETHOD(GetVertexShaderAndIALayout(MeshType meshType, ID3D11VertexShader** ppOut, ID3D11InputLayout** ppLayout)) PURE;
	STDMETHOD(CreateSwapChain(ISwapChain** out, HWND hWnd, Size2D<u32> const* bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen)) PURE;
	STDMETHOD(LoadMesh(wchar_t const* path, IMeshCollection** out)) PURE;
	STDMETHOD(CreateTexture(ITexture2D** out, D3D11_TEXTURE2D_DESC const* desc))PURE;
	STDMETHOD(LoadTexture(ITexture2D** out, wchar_t const* path))PURE;
	STDMETHOD(LoadAnimationSet(IAnimationSetCollection** out, wchar_t const* path))PURE;

#if	defined(__cplusplus)
	HRESULT CreateSwapChain(ISwapChain** out, HWND hWnd, Size2D<u32> const& bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen)
	{
		return CreateSwapChain(out, hWnd, &bufferSize, bufferFormat, fullScreen);
	}
	HRESULT CreateTexture(ITexture2D** out, D3D11_TEXTURE2D_DESC const& desc)
	{
		return CreateTexture(out, &desc);
	}
#endif
};
MIDL_INTERFACE("1C4AE506-4192-404C-9D85-0F2AB6F304B3")  ISwapChain : public IUnknown
{
public:
	//static SwapChain* Create(HWND hWindow, GraphicsModule* gModule, Size2D<u32> const& bufferSize, DXGI_FORMAT bufferFormat, bool fullScreen);
	STDMETHOD(GetBackBuffer(ITexture2D** out)) PURE;
	STDMETHOD(GetDepthBuffer(ITexture2D** out)) PURE;
	STDMETHOD(Clear(ID3D11DeviceContext* context, float const rgba[4], float depth))PURE;
	STDMETHOD(Present())PURE;
#if	defined(__cplusplus)
	HRESULT Clear(ID3D11DeviceContext* context, float const (&rgba)[4], float depth)
	{
		return Clear(context, (float const*)rgba, depth);
	}
#endif
};
//[Guid("0C5DB153-7601-4776-82F7-2DCBC85C62B0")]
MIDL_INTERFACE("0C5DB153-7601-4776-82F7-2DCBC85C62B0")  ITexture2D : public IUnknown
{
	STDMETHOD(GetResource(ID3D11Texture2D** out)) PURE;
	STDMETHOD(GetSize(Size2D<u32>* out)) PURE;
	STDMETHOD(GetView(GUID, ID3D11View** out)) PURE;
	STDMETHOD_(DXGI_FORMAT, GetFormat()) PURE;
	STDMETHOD(CreateGenerateMipmap(ID3D11Device* device, ID3D11Device* deviceContext)) PURE;
#if	defined(__cplusplus)
	template<typename T>
	HRESULT GetView(T** out)
	{
		return GerView(__uuidof(T), out);
	}
	template<typename T>
	T* GetViewRef()
	{
		T* tmp{};
		if (SUCCEEDED(GetView(__uuidof(T), &reinterpret_cast<ID3D11View*&>(tmp) )))
		{
			tmp->Release();
			return tmp;
		}
		return nullptr;
	}
#endif
};
struct __declspec(novtable) ITransform : public IUnknown
{
	STDMETHOD(SetParent(ITransform* pParent)) PURE;
	STDMETHOD(GenerateLocalMatrix(DirectX::XMFLOAT4X4* pOut) PURE);
	STDMETHOD(GenerateWorldMatrix(DirectX::XMFLOAT4X4* pOut)  PURE);
	STDMETHOD(SetPosition(DirectX::XMFLOAT3 const* pPosition) PURE);
	STDMETHOD(GetPosition(DirectX::XMFLOAT3* pOut)  PURE);
	STDMETHOD(SetScale(float scale) PURE);
	STDMETHOD(GetScale(float* out)  PURE);
	STDMETHOD(GetRosition(DirectX::XMFLOAT3* pOut) PURE);
	STDMETHOD(SetRosition(DirectX::XMFLOAT3 const* pPosition) PURE);
	STDMETHOD(Clone(ITransform** ppOut) PURE);
#if	defined(__cplusplus)
	HRESULT SetRosition(DirectX::XMFLOAT3 const& rotation)
	{
		return SetRosition(&rotation);
	}
	HRESULT SetPosition(DirectX::XMFLOAT3 const& position)
	{
		return SetPosition(&position);
	}
#endif
};
struct IMeshCollection;
struct IMesh;
//[Guid("32D84DAC-A35E-464A-B2F7-AEA8A8A10025")]
MIDL_INTERFACE("32D84DAC-A35E-464A-B2F7-AEA8A8A10025")  IMesh : public IUnknown
{
	STDMETHOD_(MeshType, GetType()) PURE;
	STDMETHOD_(u32, GetIndexCount()) PURE;
	STDMETHOD_(u32, GetIndexBase()) PURE;
	STDMETHOD_(wchar_t const*, GetMaterial()) PURE;
	STDMETHOD_(wchar_t const*, GetName()) PURE;

	STDMETHOD(GetVertexBuffer(ID3D11Buffer** out)) PURE;
	STDMETHOD(GetIndexBuffer(ID3D11Buffer** out)) PURE;

};
//[Guid("79031438-320A-472D-9726-99102E8CEC70")]
MIDL_INTERFACE("79031438-320A-472D-9726-99102E8CEC70") IMeshCollection : public IUnknown
{
	STDMETHOD_(u32, GetCount())PURE;
	STDMETHOD_(wchar_t const*, GetMaterialTextureName(wchar_t const* materialName)) PURE;
	STDMETHOD(GetMesh(u32 index, IMesh** out))PURE;
};
//[Guid("6224950C-E40C-4456-8AB1-D5957C2780EA")]
MIDL_INTERFACE("6224950C-E40C-4456-8AB1-D5957C2780EA") ISkinnedMesh : public IMesh
{
	STDMETHOD_(u32, GetBoneCount()) PURE;

};

//[Guid("8758917C-483E-4B09-9E20-54A05BF265E3")]
MIDL_INTERFACE("8758917C-483E-4B09-9E20-54A05BF265E3") IAnimationSetCollection : public IUnknown
{
	STDMETHOD_(u32, GetCount()) PURE;
	STDMETHOD(GetAnimationSet(u32 index, IAnimationSet** out)) PURE;
};
//[Guid("8F1FE2D8-0694-40A6-97AF-5D4295EF4FDC")]
MIDL_INTERFACE("8F1FE2D8-0694-40A6-97AF-5D4295EF4FDC") IAnimationSet : public IUnknown
{
	STDMETHOD_(u32, GetBoneCount())PURE;
	STDMETHOD_(u32, GetFrameCount())PURE;
	STDMETHOD_(DirectX::XMFLOAT4X4 const*, GetFrame(u32 frameIdx))PURE;
	STDMETHOD_(u32, GetFPS())PURE;
};
//[Guid("A8B28854-95D3-4E24-BCFE-C7972C4D2BA1")]
MIDL_INTERFACE("A8B28854-95D3-4E24-BCFE-C7972C4D2BA1") IAnimationController : public IUnknown
{
	STDMETHOD(InsertAnimationSet(wchar_t const* name, IAnimationSet * set))PURE;
	STDMETHOD(SetTrackWeight(u32 trackIdx, f32 weight))PURE;
	STDMETHOD(SetTrackAnimationSet(u32 trackIdx, wchar_t const* animName))PURE;
	STDMETHOD(AdvanceTime(f32 delta))PURE;
	STDMETHOD(ResetTime())PURE;
	STDMETHOD_(f32, GetTime())PURE;
	STDMETHOD_(DirectX::XMFLOAT4X4 const*, GetBoneMatrices( )) PURE;
};
struct BoneDesc
{
	u32 index;
	u32 parentIndex;
	wchar_t const* name;
};
//[Guid("E1CD1BB7-B0E9-453A-B19E-679D5E6548C0")]
MIDL_INTERFACE("E1CD1BB7-B0E9-453A-B19E-679D5E6548C0") IBoneCollection : public IUnknown
{
	STDMETHOD_(u32, GetCount()) PURE;
	STDMETHOD(FindBone(wchar_t const* name, BoneDesc* out)) PURE;
	STDMETHOD(GetBone(u32 index, BoneDesc* out)) PURE;

};
extern "C" HRESULT __declspec(dllexport) __stdcall CreateGraphicsModule(IGraphicsModule** out);