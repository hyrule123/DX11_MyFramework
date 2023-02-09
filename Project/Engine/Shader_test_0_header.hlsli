#include "Shader_header_register.hlsli"


struct VS_IN
{
    float3 vPos : POSITION; // semantic  
    //float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    //float4 vOutColor : COLOR;
    float2 vOutUV : TEXCOORD;
};