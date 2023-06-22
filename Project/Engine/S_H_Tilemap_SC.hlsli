#ifndef S_0_H_TILEMAP_SC
#define S_0_H_TILEMAP_SC

#ifdef __cplusplus

#else

#include "S_H_Register.hlsli"

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
