#ifndef S_0_H_STD2D_LIGHT
#define S_0_H_STD2D_LIGHT

#include "S_H_Register.hlsli"

#ifndef __cplusplus
#include "S_H_Func.hlsli"
#endif

// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_CBuffer_Mtrl_Scalar.INT_0              : AnimUse
// g_CBuffer_Mtrl_Scalar.VEC2_0             : AnimAtlas LeftTop
// g_CBuffer_Mtrl_Scalar.VEC2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// g_tex_1              : Normal Texture
// ============================




struct tLightColor
{
	float4 vDiffuse;
	float4 vAmbient;
};

struct tLightInfo
{
	tLightColor LightColor;

	float4 vLightWorldPos;

	float4 vLightDir; //����Ʈ����Ʈ�� ����

	float fRadius; //������ �Ǵ� ����Ʈ����Ʈ�� �Ÿ�
	float fAngle;
    INT32 LightType; //�Ʒ� ENUM���� �������.
    INT32 padding;
};


ENUM_BEGIN(eLIGHT_TYPE, int)
ENUM_MEMBER(DIRECTIONAL, int, 0)
ENUM_MEMBER(POINT, int, 1)
ENUM_MEMBER(SPOTLIGHT, int, 2)
ENUM_END

#define SHADER_STD2DMTRL_bAnimUse       MTRLDATA_PARAM_SCALAR(INT_0)
#define SHADER_STD2DMTRL_vLT			MTRLDATA_PARAM_SCALAR(VEC2_0)
#define SHADER_STD2DMTRL_vSlice         MTRLDATA_PARAM_SCALAR(VEC2_1)
#define SHADER_STD2DMTRL_vOffset        MTRLDATA_PARAM_SCALAR(VEC2_2)
#define SHADER_STD2DMTRL_vCanvasSize    MTRLDATA_PARAM_SCALAR(VEC2_3)


#ifdef __cplusplus


#else

StructuredBuffer<tLightInfo> g_SBuffer_Light2D : register(e_t_SBUFFER_LIGHT2D);

#endif

struct VS_IN
{
	float3 vLocalPos SEMANTIC(POSITION);
	float2 vUV SEMANTIC(TEXCOORD);
	UINT32 uInstID SEMANTIC(SV_InstanceID);
};

struct VS_OUT
{
	float3 vWorldPos SEMANTIC(POSITION);
	float2 vUV SEMANTIC(TEXCOORD);
	UINT32 uInstID SEMANTIC(SV_InstanceID);
	float4 vPosSV SEMANTIC(SV_Position);
};

#endif