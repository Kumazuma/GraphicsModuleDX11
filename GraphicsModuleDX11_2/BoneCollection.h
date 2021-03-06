#pragma once
#include "framework.h"
#include "interface.h"
#include <assimp\scene.h>
class BoneCollection : public IBoneCollection
{

public:
	struct Bone
	{
		std::wstring boneName;
		u32 parent;
	};

	BoneCollection(aiScene const* scene);
	u32 __stdcall GetCount();
	HRESULT __stdcall FindBone(wchar_t const* name, BoneDesc* out);
	HRESULT __stdcall GetBone(u32 index, BoneDesc* out);

	std::unordered_map<std::wstring, u32> boneNames;
	std::vector< Bone> bones;
};