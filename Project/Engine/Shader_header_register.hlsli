#include "Shader_header_struct.hlsli"

//
//
//b
//
//
cbuffer CONST_BUFFER_TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
    row_major matrix g_matWVP;
};

cbuffer CONST_BUFFER_MATERIAL : register(b1)
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
    
    
    //�ؽ�ó�� ������ �Ǵ��ϱ����� ����
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;
};

//������ ���� ������ �Ѱ��ֱ� ���� �������
cbuffer CONST_BUFFER_GLOBAL : register(b2)
{
    float2 g_Resolution;
    float g_DeltaTime;
    float g_AccTime;
};


//�ϳ��� ����ȭ ���۰� �����ϴ� �ڿ��� ���� ������ ����
//�ε��� ��ȣ = ����ȭ ������ �������� ��ȣ
//��� ���� 'SBUFFERINFO' ������ �ε��� ��ȣ�� �����ϴ� ����ü
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
cbuffer CONST_BUFFER_SBUFFER_SHAREDINFO : register(b3)
{
    tSBufferInfo g_SBufferInfo[eSBUFFER_SHARED_CBUFFER_END];
}



cbuffer g_ParticleModuleData : register(b4)
{
    float4 vSpawnColor;

    float4 vSpawnScale;

    float3 vBoxShapeScale;
    float fSphereShapeRadius;

    int SpawnShapeType; // Sphere , Box
    int SpawnRate; // �ʴ� ���� ����
    float2 Padding;

	// Color Change ���
    float4 vStartColor; // �ʱ� ����
    float4 vEndColor; // ���� ����

	// Scale Change ���
    float4 vStartScale; // �ʱ� ũ��
    float4 vEndScale; // ���� ũ��	

	// ���� �ִ�ũ��
    int iMaxParticleCount;

	// Module Check(���� int 3��¥�� �迭)
    int bModule_ParticleSpawn;
    int bModule_ColorChange;
    int bModule_ScaleChange;
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


//enum eSRV_REGISTER_IDX
//{
//    NONE = -1,
//	LIGHT2D = 8,
//	TILE = 9,
//	SETCOLOR = 10,
//	PARTICLE = 11
//};

StructuredBuffer<tLightInfo> g_Light2DSBuffer : register(t8);
StructuredBuffer<tTile> g_TileSBuffer : register(t9);
StructuredBuffer<float4> g_TestBuffer : register(t10);
StructuredBuffer<tParticle> g_ParticleInfo : register(t11);




//
//
//u
//
//

//u0: Shader_SetColor ��� ��
//eUAV_REGISTER_IDX
//{
//    NONE = -1,
//	SETCOLOR_TEXTURE,
//	SETCOLOR_SBUFFER,
//	PARTICLE_SBUFFER,
//	PARTICLE_SBUFFER_SHARED
//};

RWTexture2D<float4> g_Output : register(u0);
RWStructuredBuffer<float4> g_TestSBUffer : register(u1);
RWStructuredBuffer<tParticle> g_ParticleSBuffer : register(u2);
RWStructuredBuffer<tRWParticleBuffer> g_ParticleSBufferSharedRW : register(u3);




SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


#define COLOR_KEY g_vec4_3

#define CS_TotalCountX g_int_0
#define CS_TotalCountY g_int_1
#define CS_TotalCountZ g_int_2