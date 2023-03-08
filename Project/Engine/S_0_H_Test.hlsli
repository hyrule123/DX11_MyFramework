#ifndef S_H_TEST
#define S_H_TEST

#include "S_H_Register.hlsli"



#ifdef _cplusplus

#else

#include "S_H_Func.hlsli"

#endif


struct VS_IN
{
    float3 vPos SEMANTIC(POSITION); // semantic  
    //float4 vColor : COLOR;
	float2 vUV SEMANTIC(TEXCOORD);
    UINT32 uInstID SEMANTIC(SV_InstanceID);
};

struct VS_OUT
{
    float4 vPosition SEMANTIC(SV_Position);
    //float4 vOutColor : COLOR;
    float2 vOutUV SEMANTIC(TEXCOORD);
    UINT32 uInstID SEMANTIC(SV_InstanceID);
};

#endif