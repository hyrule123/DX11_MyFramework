#ifndef S_H_PARTICLE_BASIC
#define S_H_PARTICLE_BASIC

#include "S_H_Register.hlsli"

#define PARTICLE_BASIC_THREADS_X 128
#define PARTICLE_BASIC_THREADS_Y 1
#define PARTICLE_BASIC_THREADS_Z 1

#define REGISLOT_t_SBUFFER_PARTICLE_TRANSFORM	REGISTER_SLOT(t, 16)

#define REGISLOT_u_SBUFFERRW_PARTICLE			REGISTER_SLOT(u, 0)
#define REGISLOT_u_SBUFFERRW_PARTICLE_SHAREDATA	REGISTER_SLOT(u, 1)


struct tParticleShareData
{
    INT32 iSpawnCount; // 스폰 시킬 파티클 개수
	UINT32_2 uSeeds;
	float Padding0;
	
	float2 NoiseTexSize;
	float2 Padding1;
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

//CBuffer에서 사용하므로 공용 struct 헤더에 선언
struct tParticleBasicSpawnSetting
{
	//Module Switch + Basic Info
    BOOL bModule_Spawn;
    BOOL bModule_ColorChange;
	BOOL bModule_ScaleChange;
    BOOL bModule_Rotation;
    
	BOOL bModule_AddVelocity;
	BOOL bModule_Drag;
	BOOL bModule_NoiseForce;
    BOOL bModule_ExpandVelocity; //속도에 따라 파티클의 크기 변화시키는 모듈
    
    BOOL bModule_ApplyMass;
    BOOL bModule_ApplyGravity;
	INT32 iMaxParticleCount;
	float PADDING1;
    
    
    //Spawn Module Part
	INT32 eSpawnShapeType; // Sphere , Box
	INT32 iSpawnRate;
	INT32 bFollowing;
	float PADDING2;
    
	float4 vSpawnColor;
	float4 vSpawnScaleMin;
	float4 vSpawnScaleMax;
    
	float3 vBoxShapeScale;
	float fSphereShapeRadius;
    
	float fMinLifeTime;
	float fMaxLifeTime;
	float2 PADDING3;

    
	//Color Change Module Part
	float4 vStartColor; // 초기 색상
	float4 vEndColor; // 최종 색상

    
	// Scale Change Module Part
	float fStartScale; // 초기 크기
	float fEndScale; // 최종 크기	
	float2 PADDING4;
    
    
    //Rotation Module Part
	float3 vRotRadPerSec;
	float PADDING5;
	float3 vRotRandomRange; //이 범위 사이에서 회전속도 랜덤
	float PADDING6;

    
	// Add Velocity Module Part
	float4 vVelocityDir;
    
    INT32 eAddVelocityType; // 0 : From Center, 1 : Fixed Direction	
	float fOffsetAngle;
	float fSpeed;
	float PADDING7;

    
	// Drag Module Part : 진행될수록 속도가 감소하는 효과
	float fStartDrag;
	float fEndDrag;
    
    
    // NoiseForce 모듈
	float fNoiseTerm;
	float fNoiseForce;
        
    // Render 모듈
	INT32 VelocityAlignment; // 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
	INT32 VelocityScale; // 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
	float vMaxSpeed; // 최대 크기에 도달하는 속력
	float PADDING8;
    
    
	float4 vMaxVelocityScale; // 속력에 따른 크기 변화량 최대치
    
    
    
    //Gravity 모듈
	float fGravity;
	float fEnergyLoss;
	float Padding;
    BOOL bHeadingRight;
	
	float4 vOwnerPrevWorldPos;
	float4 vOwnerCurWorldPos;
};


#ifdef __cplusplus

#else

#include "S_H_Func.hlsli"

StructuredBuffer<tParticleTransform> g_SBuffer_ParticleTransform : register(REGISLOT_t_SBUFFER_PARTICLE_TRANSFORM);
RWStructuredBuffer<tParticleTransform> g_SBufferRW_ParticleTransform : register(REGISLOT_u_SBUFFERRW_PARTICLE);
RWStructuredBuffer<tParticleShareData> g_SBufferRW_Particle_Shared : register(REGISLOT_u_SBUFFERRW_PARTICLE_SHAREDATA);

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



#endif
