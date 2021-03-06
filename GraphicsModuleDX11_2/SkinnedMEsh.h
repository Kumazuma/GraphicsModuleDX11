#pragma once
#include"framework.h"
#include"interface.h"
#include <assimp\mesh.h>
#include <assimp\scene.h>
class SkinnedMesh : public ISkinnedMesh
{
public:
	SkinnedMesh(IGraphicsModule* const gmodule,IBoneCollection* boneCollection, aiScene const* scene, aiMesh* mesh);
	u32		__stdcall GetIndexCount() override;
	u32		__stdcall GetIndexBase() override;
	u32		__stdcall GetBoneCount() override;
	HRESULT __stdcall GetVertexBuffer(ID3D11Buffer** out) override;
	HRESULT __stdcall GetIndexBuffer(ID3D11Buffer** out) override;
	HRESULT __stdcall GetAnimationController(IAnimationController** out);
	wchar_t const* __stdcall GetMaterial() override;
	wchar_t const* __stdcall GetName()  override;
	MeshType GetType() override;
	
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> vertexBuffer;
	std::wstring name;
	std::wstring materialName;
	u32 indexCount;
	u32 boneCount;
};