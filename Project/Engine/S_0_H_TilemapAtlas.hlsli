#ifndef S_0_H_TILEMAP
#define S_0_H_TILEMAP

#include "S_H_Register.hlsli"


//Instancing : X

#define MTRL_SCALAR_TILEMAP_INT_SIZE_X MTRLDATA_PARAM_SCALAR(INT, 1)
#define MTRL_SCALAR_TILEMAP_INT_SIZE_Y MTRLDATA_PARAM_SCALAR(INT, 2)


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
