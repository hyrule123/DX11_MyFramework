#include "Shader_header_register.hlsli"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float3 vWorldPos : POSITION;
    float2 vUV    : TEXCOORD;
    float4 vSV_Pos : SV_Position;
};