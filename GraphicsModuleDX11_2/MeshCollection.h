#pragma once
#include"framework.h"
#include"interface.h"
class MeshCollection : public IMeshCollection
{
public:
	MeshCollection(IGraphicsModule* gmodule, wchar_t const* const path);
	u32	__stdcall GetCount() override;
	HRESULT __stdcall GetMesh(u32 index, IMesh** out) override;
	wchar_t const* __stdcall GetMaterialTextureName(wchar_t const* materialName) override;
	
private:
	std::vector<ComPtr<IMesh> > meshs;
	std::unordered_map<std::wstring, std::wstring > diffuseTextures;
	IGraphicsModule* gmodule;
};