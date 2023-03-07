#ifndef TILEMAP_COMPLETE_HEADER
#define TILEMAP_COMPLETE_HEADER

#ifdef __cplusplus

#else

#include "Shader_Header_Register.hlsli"

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