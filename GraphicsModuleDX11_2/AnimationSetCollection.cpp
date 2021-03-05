#include "pch.h"
#include "AnimationSetCollection.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

AnimationSetCollection::AnimationSetCollection(wchar_t const* path)
{
    Assimp::Importer importer;
    std::string filename{ ConvertWideToUTF8(path) };
    
    const aiScene* pScene = importer.ReadFile(filename,
        aiProcess_ConvertToLeftHanded);
    animationSets.reserve(pScene->mNumAnimations);
    auto it{ pScene->mAnimations };
    auto const end{ it + pScene->mNumAnimations };
    for (; it != end; ++it)
    {
        aiAnimation* anim{ *it };
        
    }
}

u32 __stdcall AnimationSetCollection::GetCount()
{
    return animationSets.size();
}

HRESULT __stdcall AnimationSetCollection::GetAnimationSet(u32 index, IAnimationSet** out)
{
    if (out == nullptr)return E_POINTER;
    if (index >= animationSets.size()) return E_INVALIDARG;
    IAnimationSet* animSet = animationSets[index].Get();
    animSet->AddRef();
    *out = animSet;
    return S_OK;
}
