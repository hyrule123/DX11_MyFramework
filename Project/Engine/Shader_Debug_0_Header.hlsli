#ifndef SHADER_DEBUG_HEADER
#define SHADER_DEBUG_HEADER

#include "Shader_header_register.hlsli"


//Debug ������ ������ Material�� �����͸� �����ؼ� ó�� ��.

#define MTRL_SCALAR_DEBUG_MAT_VP    MTRLDATA_PARAM_SCALAR(MAT_0)
#define MTRL_SCALAR_DEBUG_MAT_WORLD MTRLDATA_PARAM_SCALAR(MAT_1)

#define MTRL_SCALAR_DEBUG_VEC4_COLOR     MTRLDATA_PARAM_SCALAR(VEC4_0)


#ifdef __cplusplus

#else

#include "Shader_Func.hlsli"

struct VS_IN
{
	float3 vPos SEMANTIC(POSITION); // semantic  
	UINT32 uInstID SEMANTIC(SV_InstanceID);
};

struct VS_OUT
{
	float4 vPos SEMANTIC(SV_Position);
	UINT32 uInstID SEMANTIC(SV_InstanceID);
};

#endif

#endif