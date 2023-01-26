#include "Shader_header_register.hlsli"

#define COLOR_KEY g_vec4_3

#define eLIGHT_DIRECTIONAL  (uint)0	//직사광선
#define eLIGHT_POINT        (uint)1	//점광원
#define eLIGHT_SPOTLIGHT    (uint)2	//스포트라이트

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float3 vWorldPos : POSITION;
    float2 vUV    : TEXCOORD;
    float4 vSV_Pos : SV_Position;
};