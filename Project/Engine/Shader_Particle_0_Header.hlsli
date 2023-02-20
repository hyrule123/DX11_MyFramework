#ifndef SHADER_PARTICLE_HEADER
#define SHADER_PARTICLE_HEADER

#include "Shader_header_register.hlsli"



struct tParticleShareData
{
    INT32 iSpawnCount; // ���� ��ų ��ƼŬ ����
	UINT32_2 uSeeds;
	float Padding;
};


// Particle
struct tParticleTransform
{
	//��ġ ����
	float4 vLocalPos;
	float4 vWorldScale; // ��ƼŬ ũ��
	float4 vWorldRotation;
	
	float4 vWorldPos; // ��ƼŬ ��ġ
	float4 vVelocity; // ��ƼŬ ���� �ӵ�
	float4 vForce; // ��ƼŬ�� �־��� ��
	
	float4 vColor; // ��ƼŬ ����

	float fAge; // ���� �ð�
	float fPrevAge;
	float fNormalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
	float fLifeTime; // ����
	
	float fMass; // ����
	float fScaleFactor; // �߰� ũ�� ����

	BOOL bActive;
	BOOL bIsBouncing;
	
	float4 vRotPerSec;
};


#ifdef __cplusplus

#else

StructuredBuffer<tParticleTransform> g_SBuffer_ParticleTransform : register(e_t_SBUFFER_PARTICLE_TRANSFORM);

#endif

struct VS_INOUT
{
	float3 vPos SEMANTIC(POSITION);
	UINT32 uInstID SEMANTIC(SV_InstanceID);
};

//Geometry Shader �� ��� ����
//1. ���������� ����(�ȼ� ���̴��� �����ϱ� ���� ���̴� ������������ �ߴ�)
//2. ������ ó��(� ���¿����� ī�޶� �ٶ�)
struct GS_OUT
{
	float4 vSV_Pos SEMANTIC(SV_Position);
	float2 vUV SEMANTIC(TEXCOORD);
	UINT32 uInstID SEMANTIC(SV_InstanceID);
};



#ifndef __cplusplus

#include "Shader_Func.hlsli"
RWStructuredBuffer<tParticleTransform> g_SBufferRW_ParticleTransform : register(e_u_PARTICLE_SBUFFERRW);
RWStructuredBuffer<tParticleShareData> g_SBufferRW_Particle_Shared : register(e_u_PARTICLE_SBUFFERRW_SHAREDATA);

#endif

#endif