#pragma once
#include<DirectXMath.h>
#if defined(__cplusplus)
#define ENUM enum class
#endif
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

typedef DirectX::XMFLOAT2 f32_2;
typedef DirectX::XMFLOAT3 f32_3;
typedef DirectX::XMFLOAT4 f32_4;
typedef DirectX::XMFLOAT4X4 f32_44;

ENUM MeshType
{
	Static,
	Skinned
};

template<typename Type>
struct Size2D
{
	Type width;
	Type height;
};