#ifndef SHADER_TILEMAP_HEADER
#define SHADER_TILEMAP_HEADER

#include "Shader_header_register.hlsli"


//Instancing : X



struct tTile
{
	float2 vLeftTop;
	float2 vSlice;
};


#ifdef __cplusplus

#else

StructuredBuffer<tTile> g_SBuffer_Tile : register(e_t_SBUFFER_TILE);

#endif

struct VTX_TILEMAP_IN
{
    float3 vPos SEMANTIC(POSITION);
    float2 vUV SEMANTIC(TEXCOORD);
};

struct VTX_TILEMAP_OUT
{
    float4 vPosition SEMANTIC(SV_Position);
    float2 vUV SEMANTIC(TEXCOORD);
};

#endif