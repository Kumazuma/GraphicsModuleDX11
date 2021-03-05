#pragma once
#include"framework.h"
#include"interface.h"

class AnimationSetCollection : public IAnimationSetCollection
{
public:
	AnimationSetCollection(wchar_t const* path);
	u32 __stdcall GetCount();
	HRESULT __stdcall GetAnimationSet(u32 index, IAnimationSet** out);
	std::vector<ComPtr<IAnimationSet> > animationSets;
};