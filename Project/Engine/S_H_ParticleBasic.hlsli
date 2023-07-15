#ifndef S_0_H_PARTICLE
#define S_0_H_PARTICLE

#include "S_H_Register.hlsli"

#define PARTICLE_BASIC_THREADS_X 128
#define PARTICLE_BASIC_THREADS_Y 1
#define PARTICLE_BASIC_THREADS_Z 1

#define REGISLOT_u_SBUFFERRW_PARTICLE			REGISTER_SLOT(u, 0)
#define REGISLOT_u_SBUFFERRW_PARTICLE_SHAREDATA	REGISTER_SLOT(u, 1)

struct tParticleShareData
{
    INT32 iSpawnCount; // 스폰 시킬 파티클 개수
	UINT32_2 uSeeds;
	float Padding;
};


// Particle
struct tParticleTransform
{
	//위치 정보
	float4 vLocalPos;
	float4 vWorldScale; // 파티클 크기
	float4 vWorldRotation;
	
	float4 vWorldPos; // 파티클 위치
	float4 vVelocity; // 파티클 현재 속도
	float4 vForce; // 파티클에 주어진 힘
	
	float4 vColor; // 파티클 색상

	float fAge; // 생존 시간
	float fPrevAge;
	float fNormalizedAge; // 수명대비 생존시간을 0~1로 정규화 한 값
	float fLifeTime; // 수명
	
	float fMass; // 질량
	float fScaleFactor; // 추가 크기 배율

	BOOL bActive;
	BOOL bIsBouncing;
	
	float4 vRotPerSec;
};


#ifdef __cplusplus

#else

StructuredBuffer<tParticleTransform> g_SBuffer_ParticleTransform : register(REGISLOT_t_SBUFFER_PARTICLE_TRANSFORM);

#endif

struct VS_INOUT
{
	float3 vPos SEMANTIC(POSITION);
	UINT32 uInstID SEMANTIC(SV_InstanceID);
};

//Geometry Shader 의 사용 목적
//1. 파이프라인 제어(픽셀 쉐이더에 도달하기 전에 쉐이더 파이프라인을 중단)
//2. 빌보드 처리(어떤 상태에서든 카메라를 바라봄)
struct GS_OUT
{
	float4 vSV_Pos SEMANTIC(SV_Position);
	float2 vUV SEMANTIC(TEXCOORD);
	UINT32 uInstID SEMANTIC(SV_InstanceID);
};



#ifndef __cplusplus

#include "S_H_Func.hlsli"
RWStructuredBuffer<tParticleTransform> g_SBufferRW_ParticleTransform : register(REGISLOT_u_SBUFFERRW_PARTICLE);
RWStructuredBuffer<tParticleShareData> g_SBufferRW_Particle_Shared : register(REGISLOT_u_SBUFFERRW_PARTICLE_SHAREDATA);

#endif

#endif
