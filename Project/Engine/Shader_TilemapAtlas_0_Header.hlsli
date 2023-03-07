#ifndef SHADER_TILEMAP_HEADER
#define SHADER_TILEMAP_HEADER

#include "Shader_header_register.hlsli"


//Instancing : X

#define MTRL_SCALAR_TILEMAP_INT_SIZE_X MTRLDATA_PARAM_SCALAR(INT_1)
#define MTRL_SCALAR_TILEMAP_INT_SIZE_Y MTRLDATA_PARAM_SCALAR(INT_2)


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