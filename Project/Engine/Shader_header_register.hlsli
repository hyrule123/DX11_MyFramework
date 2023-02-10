#ifndef SHADER_HEADER_REGISTER
#define SHADER_HEADER_REGISTER


#include "Shader_header_struct.hlsli"



//
//
//b
//
//
cbuffer CBuffer_Transform : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
    row_major matrix g_matWVP;
};

cbuffer CBuffer_Material : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;

    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    
    //텍스처의 유무를 판단하기위한 변수
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;
};

//게임의 각종 정보를 넘겨주기 위한 상수버퍼
cbuffer CBuffer_Global : register(b2)
{
    float2 g_Resolution;
    float g_DeltaTime;
    float g_AccTime;
};


//하나의 구조화 버퍼가 공유하는 자원에 대한 정보를 저장
//인덱스 번호 = 구조화 버퍼의 레지스터 번호
//상수 버퍼 'SBUFFERINFO' 내부의 인덱스 번호를 지정하는 열거체
//enum eSBUFFER_SHARED_CBUFFER_IDX : int
//{
//	NONE = -1,
//	LIGHT2D,
//	TILE,
//	SETCOLOR,
//	PARTICLE,
//	END
//};
#define eSBUFFER_SHARED_CBUFFER_LIGHT2D 0u
#define eSBUFFER_SHARED_CBUFFER_TILE 1u
#define eSBUFFER_SHARED_CBUFFER_SETCOLOR 2u
#define eSBUFFER_SHARED_CBUFFER_PARTICLE 3u
#define eSBUFFER_SHARED_CBUFFER_END 4u
cbuffer CBuffer_SBuffer_SharedInfo : register(b3)
{
    tSBufferInfo g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_END];
}



cbuffer CBuffer_ParticleModule : register(b4)
{
   	// 스폰 모듈
    float4 g_vSpawnColor;
    float4 g_vSpawnScale;
    float3 g_vBoxShapeScale;
    float  g_fSphereShapeRadius;
	INT32  g_eSpawnShapeType; // Sphere , Box
	INT32  g_iSpawnRate;
	INT32  g_bFollowing; // 0 World, 1 Local
	INT32  Padding0;

	// Color Change 모듈
    float4 g_vStartColor; // 초기 색상
    float4 g_vEndColor; // 최종 색상

	// Scale Change 모듈
    float4 g_vStartScale; // 초기 크기
    float4 g_vEndScale; // 최종 크기	

	// Module Check
	INT32  g_bSpawn;
	INT32  g_bColorChange;
	INT32  g_bScaleChange;
	INT32  Padding1;

	INT32  g_iMaxParticleCount;
    float3 Padding2;
}




//
//
//t
//
//
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

TextureCube g_cube_0 : register(t8);
TextureCube g_cube_1 : register(t9);

Texture2DArray g_arrtex_0 : register(t10);
Texture2DArray g_arrtex_1 : register(t11);



StructuredBuffer<tLightInfo> g_Light2DSBuffer : register(t12);
StructuredBuffer<tTile> g_TileSBuffer : register(t13);
StructuredBuffer<float4> g_TestBuffer : register(t14);
StructuredBuffer<tParticle> g_ParticleInfo : register(t15);
Texture2D g_Tex_Noise : register(t16);




//
//
//u
//
//

//u0: Shader_SetColor 사용 중
//eUAV_REGISTER_IDX
//{
//    NONE = -1,
//	SETCOLOR_TEXTURE,
//	SETCOLOR_SBUFFER,
//	PARTICLE_SBUFFER,
//	PARTICLE_SBUFFER_SHARED
//};

RWTexture2D<float4> g_TexRW_Output : register(u0);
RWStructuredBuffer<float4> g_SBufferRW_Test : register(u1);
RWStructuredBuffer<tParticle> g_SBufferRW_Particle : register(u2);
RWStructuredBuffer<tRWParticleBuffer> g_SBufferRW_Particle_Shared : register(u3);




SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


#define COLOR_KEY g_vec4_3

#define CS_TotalCountX g_int_0
#define CS_TotalCountY g_int_1
#define CS_TotalCountZ g_int_2


#endif