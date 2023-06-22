#ifndef S_0_H_TILEMAP_COMPLETE
#define S_0_H_TILEMAP_COMPLETE

#ifdef __cplusplus

struct tMapInfo
{
    UINT32 uNumMegatileX;
    UINT32 uNumMegatileY;
    eTILESET_INFO eTileSet;
    UINT32 Padding;
};

#else

#include "S_H_Register.hlsli"

struct tMapInfo
{
	UINT32 uNumMegatileX;
	UINT32 uNumMegatileY;
	UINT32 eTileSet;
	UINT32 Padding;
};

#endif


struct VS_IN
{
	float3 vLocalPos SEMANTIC(POSITION);
	float2 vUV    SEMANTIC(TEXCOORD);
};

struct VS_OUT
{
	float4 vWorldPos SEMANTIC(SV_Position);
	float2 vUV		SEMANTIC(TEXCOORD);
};





#endif
