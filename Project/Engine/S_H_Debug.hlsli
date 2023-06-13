#ifndef S_0_H_DEBUG
#define S_0_H_DEBUG

#include "S_H_Register.hlsli"


//Debug 쉐이프 정보는 Material로 데이터를 전송해서 처리 중.

#define MTRL_SCALAR_DEBUG_MAT_WVP    MTRLDATA_PARAM_SCALAR(MAT, 0)
#define MTRL_SCALAR_DEBUG_MAT_WORLD MTRLDATA_PARAM_SCALAR(MAT, 1)

#define MTRL_SCALAR_DEBUG_INT_SHAPETYPE MTRLDATA_PARAM_SCALAR(INT, 1)

#define MTRL_SCALAR_DEBUG_VEC4_COLOR     MTRLDATA_PARAM_SCALAR(VEC4, 0)

ENUM_BEGIN(eDEBUGSHAPE_TYPE, int)
	ENUM_MEMBER(RECT, int, 0)
	ENUM_MEMBER(CIRCLE, int, 1)
	ENUM_MEMBER(CUBE, int, 2)
	ENUM_MEMBER(SPHERE, int, 3)
	ENUM_MEMBER(END, int, 4)
ENUM_END


#ifdef __cplusplus

#else

#include "S_H_Func.hlsli"

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
