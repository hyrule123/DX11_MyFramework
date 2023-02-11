#include "Shader_header_register.hlsli"

#define Debug_matWVP g_CBuffer_MtrlData.mat_0
#define Debug_vecColor g_CBuffer_MtrlData.vec4_0

//Debug ������ ������ Material�� ����ؼ� ��� ��.

struct VS_IN
{
    float3 vPos : POSITION; // semantic  
};

struct VS_OUT
{
    float4 vPos : SV_Position;
};
