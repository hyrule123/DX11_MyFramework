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
    tTransform g_CBuffer_Transform;
};

cbuffer CBuffer_Material : register(b1)
{
    tMtrlData g_CBuffer_MtrlData;
};

//게임의 각종 정보를 넘겨주기 위한 상수버퍼
cbuffer CBuffer_Global : register(b2)
{
    tGlobalData g_CBuffer_GlobalData;
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
    tParticleModule g_CBuffer_ParticleModule;
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



StructuredBuffer<tLightInfo> g_SBuffer_Light2D : register(t12);
StructuredBuffer<tTile> g_SBuffer_Tile : register(t13);
StructuredBuffer<float4> g_SBuffer_Test : register(t14);
StructuredBuffer<tParticleTransform> g_SBuffer_ParticleTransform : register(t15);
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
RWStructuredBuffer<tParticleTransform> g_SBufferRW_Particle : register(u2);
RWStructuredBuffer<tRWParticleBuffer> g_SBufferRW_Particle_Shared : register(u3);




SamplerState g_Sampler_0 : register(s0);
SamplerState g_Sampler_1 : register(s1);


#define COLOR_KEY g_CBuffer_MtrlData.vec4_3

#define CS_TOTAL_ELEMCOUNT_X g_CBuffer_MtrlData.int_0
#define CS_TOTAL_ELEMCOUNT_Y g_CBuffer_MtrlData.int_1
#define CS_TOTAL_ELEMCOUNT_Z g_CBuffer_MtrlData.int_2


#endif