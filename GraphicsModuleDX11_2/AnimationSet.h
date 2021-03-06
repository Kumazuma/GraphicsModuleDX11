#pragma once
#include"framework.h"
#include"interface.h"


class AnimationSet : public IAnimationSet
{
	u32 __stdcall GetBoneCount();
	u32 __stdcall GetFrameCount();
	f32_44 const* __stdcall GetFrame(u32 frameIdx);
	u32 __stdcall GetFPS();
};