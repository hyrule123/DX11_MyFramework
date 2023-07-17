#ifndef S_H_REGISTER
#define S_H_REGISTER

#include "S_H_Struct.hlsli"

#ifdef __cplusplus
#define REGISTER_SLOT(_RegisterType, _iSlotIdx) _iSlotIdx
#else
#define REGISTER_SLOT(_RegisterType, _iSlotIdx) _RegisterType##_iSlotIdx
#endif


//==========================================
//	   CONSTANT BUFFERS ( b Register )
//==========================================

//Register Number
#define REGISLOT_b_CBUFFER_CAM_MATIRCES				REGISTER_SLOT(b, 0)
#define REGISLOT_b_CBUFFER_MTRL_SCALAR				REGISTER_SLOT(b, 1)
#define REGISLOT_b_CBUFFER_MTRL_TEX					REGISTER_SLOT(b, 2)
#define REGISLOT_b_CBUFFER_GLOBAL					REGISTER_SLOT(b, 3)
#define REGISLOT_b_CBUFFER_SBUFFER_SHAREDATA		REGISTER_SLOT(b, 4)
#define REGISLOT_b_CBUFFER_COMPUTE_SHADER_INFO		REGISTER_SLOT(b, 5)
#define REGISLOT_b_END								6

	



#ifdef __cplusplus
	

//아래의 CBuffer_SBUFFER_SHARED_DATA에 전달되는 상수버퍼
extern tSBufferInfo g_SBufferShareData;

#else


cbuffer CBuffer_CamMatrices : register(REGISLOT_b_CBUFFER_CAM_MATIRCES)
{
	tCamMatrices g_CBuffer_matCam[eCAMERA_INDEX::END];
};

cbuffer CBuffer_Material_Scalar : register(REGISLOT_b_CBUFFER_MTRL_SCALAR)
{
	tMtrlScalarData g_CBuffer_Mtrl_Scalar;
};

cbuffer CBuffer_Material_Tex : register(REGISLOT_b_CBUFFER_MTRL_TEX)
{
	tMtrlTexData g_CBuffer_Mtrl_Tex;
};

	//게임의 각종 정보를 넘겨주기 위한 상수버퍼
cbuffer CBuffer_Global : register(REGISLOT_b_CBUFFER_GLOBAL)
{
	tGlobalValue g_CBuffer_GlobalData;
};
	 
cbuffer CBuffer_SBUFFER_SHARED_DATA : register(REGISLOT_b_CBUFFER_SBUFFER_SHAREDATA)
{
	tSBufferInfo g_CBuffer_SBufferData;
}


	#endif


//==================================================
// SRV : cTexture / Structured Buffers ( t Register )
//==================================================

#define REGISLOT_t_TEXTURE_0 REGISTER_SLOT(t, 0)
#define REGISLOT_t_TEXTURE_1 REGISTER_SLOT(t, 1)
#define REGISLOT_t_TEXTURE_2 REGISTER_SLOT(t, 2)
#define REGISLOT_t_TEXTURE_3 REGISTER_SLOT(t, 3)
#define REGISLOT_t_TEXTURE_4 REGISTER_SLOT(t, 4)
#define REGISLOT_t_TEXTURE_5 REGISTER_SLOT(t, 5)
#define REGISLOT_t_TEXTURE_6 REGISTER_SLOT(t, 6)
#define REGISLOT_t_TEXTURE_7 REGISTER_SLOT(t, 7)

#ifdef __cplusplus
#define eMTRLDATA_TEXPARAM_END 8
#endif

#define REGISLOT_t_SBUFFER_MTRL_SCALAR REGISTER_SLOT(t, 8)

#define REGISLOT_t_TEXTURE_CUBE_0 REGISTER_SLOT(t, 9)
#define REGISLOT_t_TEXTURE_CUBE_1 REGISTER_SLOT(t, 10)

#define REGISLOT_t_TEXTURE_ARRAY_0 REGISTER_SLOT(t, 11)
#define REGISLOT_t_TEXTURE_ARRAY_1 REGISTER_SLOT(t, 12)

#define REGISLOT_t_SBUFFER_LIGHT2D				REGISTER_SLOT(t, 13)
#define REGISLOT_t_SBUFFER_TILE					REGISTER_SLOT(t, 14)


#define REGISLOT_t_TEXUTRE_NOISE				REGISTER_SLOT(t, 17)

//초기 설정을 계속 바인딩해 두어야 하므로 겹치지 않게 127번과 바인딩
//DX11의 경우 t 버퍼는 최대 128(0~127)까지 지원한다고 함
#define REGISLOT_t_INIT_SETTING			REGISTER_SLOT(t, 127)

	#ifdef __cplusplus

#define REGISLOT_t_SRV_NONE -1


	#else

Texture2D g_tex_0 : register(REGISLOT_t_TEXTURE_0);
Texture2D g_tex_1 : register(REGISLOT_t_TEXTURE_1);
Texture2D g_tex_2 : register(REGISLOT_t_TEXTURE_2);
Texture2D g_tex_3 : register(REGISLOT_t_TEXTURE_3);
Texture2D g_tex_4 : register(REGISLOT_t_TEXTURE_4);
Texture2D g_tex_5 : register(REGISLOT_t_TEXTURE_5);
Texture2D g_tex_6 : register(REGISLOT_t_TEXTURE_6);
Texture2D g_tex_7 : register(REGISLOT_t_TEXTURE_7);

StructuredBuffer<tMtrlScalarData> g_SBuffer_Mtrl_Scalar : register(REGISLOT_t_SBUFFER_MTRL_SCALAR);

TextureCube g_cube_0 : register(REGISLOT_t_TEXTURE_CUBE_0);
TextureCube g_cube_1 : register(REGISLOT_t_TEXTURE_CUBE_1);

Texture2DArray g_arrtex_0 : register(REGISLOT_t_TEXTURE_ARRAY_0);
Texture2DArray g_arrtex_1 : register(REGISLOT_t_TEXTURE_ARRAY_1);

Texture2D g_Tex_Noise : register(REGISLOT_t_TEXUTRE_NOISE);

	#endif


//=========================================================
// UAV : RW Structured Buffer / RW cTexture ( u Register)
//=========================================================
//UAV -> u Register


//u 레지스터는 DX11 기준 0 ~ 7번까지만 존재하므로 참고





	#ifdef __cplusplus

#define REGISLOT_u_UAV_NONE -1

	#else

//각자 사용하는 쉐이더에서 별도로 선언하였음.
//u0
////g_TexRW_Output
////g_SBufferRW_ParticleTransform

//u1
////g_SBufferRW_Test
////g_SBufferRW_Particle_Shared

	#endif



//==================================
//			SAMPLER
//==================================

#define REGISLOT_S_SAMPLER_ANISOTROPIC	REGISTER_SLOT(s, 0)
#define REGISLOT_S_SAMPLER_POINT		REGISTER_SLOT(s, 1)

#ifdef __cplusplus

#define REGISLOT_S_SAMPLER_END 2

#else

SamplerState g_Sampler_0 : register(REGISLOT_S_SAMPLER_ANISOTROPIC);
SamplerState g_Sampler_1 : register(REGISLOT_S_SAMPLER_POINT);

#endif






//==================================
//				ETC
//==================================
#ifdef __cplusplus


#else


#endif


#endif

