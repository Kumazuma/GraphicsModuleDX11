#include "pch.h"
#include "SkinnedMEsh.h"
#include "typedef.h"
#include <array>
struct SkinnedMeshVertex
{
    DirectX::XMFLOAT3 vPos;
    DirectX::XMFLOAT2 vTex;
    DirectX::XMFLOAT3 vNormal;
    std::array<f32, 4> boneWeights;
    std::array<u32, 4> boneIndices;
};
SkinnedMesh::SkinnedMesh(IGraphicsModule* const gmodule, aiScene const* scene, aiMesh* mesh)
{
    ComPtr<ID3D11Device> device;
    std::vector<SkinnedMeshVertex> vertices;
    std::vector<u32> indices;
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);
    gmodule->GetDevice(&device);
    materialName = ConvertUTF8ToWide(scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str());
    name = ConvertUTF8ToWide(mesh->mName.C_Str());
    std::vector<std::pair<u32, f32>> weights;
    weights.reserve(256);
    for (u32 i = 0; i < mesh->mNumVertices; ++i)
    {
        weights.clear();
        SkinnedMeshVertex vertex;
        vertex.vPos = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };
        vertex.vTex = {
            mesh->mTextureCoords[0][i].x,
            mesh->mTextureCoords[0][i].y
        };
        vertex.vNormal = {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        };

        u32 meshBoneCount = mesh->mNumBones;
        boneCount = mesh->mNumBones;
        for (u32 j = 0; j < meshBoneCount; ++j)
        {
            auto bone{ mesh->mBones[j] };
            f32 weight{};
            bool found{ false };
            u32 weightCount{ bone->mNumWeights };
            for (u32 k = 0; k < weightCount; ++k)
            {
                if (bone->mWeights[k].mVertexId == i)
                {
                    found = true;
                    weight = bone->mWeights[k].mWeight;
                }
            }
            if (found)
            {
                weights.emplace_back(j, weight);
            }
        }
        if (weights.size() > 4)
        {
            std::sort(weights.begin(), weights.end(),
                [](std::pair<u32, f32> const& lhs, std::pair<u32, f32> const& rhs)->bool
                {
                    return lhs.second > rhs.second;
                });
            std::array<std::pair<u32, f32>, 4> newWeight(*reinterpret_cast<std::array<std::pair<u32, f32>, 4>const*>(weights.data()));
            f32 sum{};
            for (std::pair<u32, f32> it : newWeight)
            {
                sum += it.second;
            }
            for (std::pair<u32, f32>& it : newWeight)
            {
                it.second = it.second / sum;
            }
            weights.assign(newWeight.begin(), newWeight.end());
        }
        u32 boneCount = std::min(weights.size(), (size_t)4);
        for (u32 i = 0; i < boneCount; ++i)
        {
            vertex.boneIndices[i] = weights[i].first;
            vertex.boneWeights[i] = weights[i].second;
        }
        vertices.push_back(vertex);
    }
    for (u32 i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];

        for (u32 j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    auto vertexBufferDesc{ CD3D11_BUFFER_DESC(sizeof(SkinnedMeshVertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER) };
    auto indexBufferDesc{ CD3D11_BUFFER_DESC(sizeof(u32) * indices.size(), D3D11_BIND_INDEX_BUFFER) };
    D3D11_SUBRESOURCE_DATA subResource{};
    HRESULT hr{};
    subResource.pSysMem = vertices.data();
    hr = device->CreateBuffer(&vertexBufferDesc, &subResource, &vertexBuffer);

    subResource.pSysMem = indices.data();
    hr = device->CreateBuffer(&indexBufferDesc, &subResource, &indexBuffer);

    indexCount = indices.size();
}

u32 __stdcall SkinnedMesh::GetIndexCount()
{
    return indexCount;
}

u32 __stdcall SkinnedMesh::GetIndexBase()
{
    return 0u;
}

u32 __stdcall SkinnedMesh::GetBoneCount()
{
    return boneCount;
}

HRESULT __stdcall SkinnedMesh::GetVertexBuffer(ID3D11Buffer** out)
{
    return E_NOTIMPL;
}

HRESULT __stdcall SkinnedMesh::GetIndexBuffer(ID3D11Buffer** out)
{
    return E_NOTIMPL;
}

HRESULT __stdcall SkinnedMesh::GetAnimationController(IAnimationController** out)
{
    return E_NOTIMPL;
}

wchar_t const* __stdcall SkinnedMesh::GetMaterial()
{
    return materialName.c_str();
}

wchar_t const* __stdcall SkinnedMesh::GetName()
{
    return name.c_str();
}

MeshType SkinnedMesh::GetType()
{
    return MeshType::Skinned;
}
