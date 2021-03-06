#include "pch.h"
#include "BoneCollection.h"
#include <numeric>
void BuildBoneTree(aiNode* node, std::vector<BoneCollection::Bone>& bones, std::unordered_map<std::wstring, u32>& nameTable);
BoneCollection::BoneCollection(aiScene const* const scene)
{
	BuildBoneTree(scene->mRootNode, bones, boneNames);
}

u32 __stdcall BoneCollection::GetCount()
{
	return bones.size();
}

HRESULT __stdcall BoneCollection::FindBone(wchar_t const* name, BoneDesc* out)
{
	if (out == nullptr)return E_POINTER;
	if (name == nullptr)return E_INVALIDARG;
	auto it = boneNames.find(name);
	if (it == boneNames.end())return E_INVALIDARG;
	out->index = it->second;
	out->name = bones[it->second].boneName.c_str();
	out->parentIndex = bones[it->second].parent;
	return S_OK;
}

HRESULT __stdcall BoneCollection::GetBone(u32 index, BoneDesc* out)
{
	if (out == nullptr)return E_POINTER;
	if (bones.size() <= index)return E_INVALIDARG;
	out->index = index;
	out->name = bones[index].boneName.c_str();
	out->parentIndex = bones[index].parent;
	return S_OK;
}

void BuildBoneTree(aiNode* node, std::vector<BoneCollection::Bone>& bones, std::unordered_map<std::wstring, u32>& nameTable)
{
	BoneCollection::Bone bone{};
	bone.parent = std::numeric_limits<u32>::max();
	if (node->mParent != nullptr)
	{
		std::wstring parentName = ConvertUTF8ToWide(node->mParent->mName.C_Str());
		auto const it = nameTable.find(parentName);
		if (it != nameTable.end())
		{
			bone.parent = it->second;
		}
	}
	bone.boneName = ConvertUTF8ToWide(node->mName.C_Str());
	nameTable.emplace(bone.boneName, static_cast<u32>(bones.size()));
	bones.emplace_back(bone);
	auto it = node->mChildren;
	auto const end = it + node->mNumChildren;
	for (; it != end; ++it)
	{
		auto node = *it;
		BuildBoneTree(node, bones, nameTable);
	}
}
