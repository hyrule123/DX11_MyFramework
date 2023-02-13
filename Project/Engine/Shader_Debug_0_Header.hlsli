#ifndef SHADER_DEBUG_HEADER
#define SHADER_DEBUG_HEADER

#include "Shader_header_register.hlsli"


//Debug ������ ������ Material�� �����͸� �����ؼ� ó�� ��.
#define SHADER_DEBUG_matWVP MTRLDATA_PARAM_SCALAR(MAT_0)
#define SHADER_DEBUG_vecColor MTRLDATA_PARAM_SCALAR(VEC4_0)


#ifdef __cplusplus

#else

#endif



struct VS_IN
{
    float3 vPos SEMANTIC(POSITION); // semantic  
};

struct VS_OUT
{
    float4 vPos SEMANTIC(SV_Position);
};


#endif