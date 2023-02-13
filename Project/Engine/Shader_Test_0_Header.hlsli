#ifndef SHADER_TEST_HEADER
#define SHADER_TEST_HEADER

#include "Shader_header_register.hlsli"

#ifdef _cplusplus

#else

#endif


struct VS_IN
{
    float3 vPos SEMANTIC(POSITION); // semantic  
    //float4 vColor : COLOR;
    float2 vUV SEMANTIC(TEXCOORD);
};

struct VS_OUT
{
    float4 vPosition SEMANTIC(SV_Position);
    //float4 vOutColor : COLOR;
    float2 vOutUV SEMANTIC(TEXCOORD);
};

#endif