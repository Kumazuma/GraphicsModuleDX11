#include "pch.h"
#include "MeshCollection.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <filesystem>
#include "StaticMesh.h"
#include "SkinnedMEsh.h"
MeshCollection::MeshCollection(IGraphicsModule* gmodule, wchar_t const* const path)
{
	Assimp::Importer importer;
	std::string filename{ ConvertWideToUTF8(path) };
	std::filesystem::path filePath{ filename };

	const aiScene* pScene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr)
	{
		throw std::exception();
	}
	aiMesh** it{ pScene->mMeshes };
	aiMesh** const end{ pScene->mMeshes + pScene->mNumMeshes };
	std::vector <ComPtr<IMesh> > meshs;
	for (; it != end; ++it)
	{
		aiMesh* mesh{ *it };
		if (mesh->HasBones())
		{
			//TODO Skinned Mesh
			auto skinnedMesh{ new RefCountImpl<SkinnedMesh, ISkinnedMesh, IMesh, IUnknown>(gmodule,pScene, mesh) };
			meshs.emplace_back(std::move(skinnedMesh));
			skinnedMesh->Release();
		}
		else
		{
			auto sataticMesh{ new RefCountImpl<StaticMesh, IMesh, IUnknown>(gmodule,pScene, mesh) };
			meshs.emplace_back(std::move(sataticMesh));
			sataticMesh->Release();
		}
	}
	auto materialIt = pScene->mMaterials;
	auto const materialEnd = pScene->mMaterials + pScene->mNumMaterials;
	for (; materialIt != materialEnd; ++materialIt)
	{
		auto mat = *materialIt;
		auto matName{ ConvertUTF8ToWide(mat->GetName().C_Str()) };
		for (UINT i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
		{
			aiString str;
			mat->GetTexture(aiTextureType_DIFFUSE, i, &str);

			std::wstring textureName{ ConvertUTF8ToWide(str.C_Str()) };
			diffuseTextures.emplace(matName, textureName);
		}
	}


	this->gmodule = gmodule;
	this->meshs.swap(meshs);
}

u32 __stdcall MeshCollection::GetCount()
{
    return static_cast<u32>( this->meshs.size());
}

HRESULT __stdcall MeshCollection::GetMesh(u32 index, IMesh** out)
{
	if (out == nullptr)return E_POINTER;
	if (index >= meshs.size()) return E_INVALIDARG;
	IMesh* mesh = meshs[index].Get();
	mesh->AddRef();
	*out = mesh;
    return S_OK;
}

wchar_t const* __stdcall MeshCollection::GetMaterialTextureName(wchar_t const* materialName)
{
	auto const it{ diffuseTextures.find(materialName) };
	if (it == diffuseTextures.end())
	{
		return it->second.c_str();
	}
    return nullptr;
}
