#include "Shader_header_register.hlsli"

#define Debug_matWVP g_CBuffer_MtrlData.MAT_0
#define Debug_vecColor g_CBuffer_MtrlData.VEC4_0

//Debug 쉐이프 정보는 Material을 사용해서 출력 중.

struct VS_IN
{
    float3 vPos : POSITION; // semantic  
};

struct VS_OUT
{
    float4 vPos : SV_Position;
};
