#ifndef SHADER_HEADER_STRUCT
#   define SHADER_HEADER_STRUCT

//Header for global values or struct

#define INT32 int
#define UINT32 uint
#define BOOL int
#define MATRIX row_major matrix

struct tMtrlData
{
    int int_0;
    int int_1;
    int int_2;
    int int_3;
    
    float float_0;
    float float_1;
    float float_2;
    float float_3;
    
    float2 vec2_0;
    float2 vec2_1;
    float2 vec2_2;
    float2 vec2_3;

    float4 vec4_0;
    float4 vec4_1;
    float4 vec4_2;
    float4 vec4_3;

    MATRIX mat_0;
    MATRIX mat_1;
    MATRIX mat_2;
    MATRIX mat_3;
    
    
    //텍스처의 유무를 판단하기위한 변수
    int btex_0;
    int btex_1;
    int btex_2;
    int btex_3;
    int btex_4;
    int btex_5;
    int btex_6;
    int btex_7;
};

struct tTransform
{
    MATRIX matWorld;
    MATRIX matView;
    MATRIX matProj;
    MATRIX matWVP;
};

struct tGlobalData
{
    float2 vResolution;
    float fDeltaTime;
    float fAccTime;
};

struct tLightColor
{
    float4 vDiffuse;
    float4 vAmbient;
};

struct tLightInfo
{
    tLightColor LightColor;

    float4 vLightWorldPos;

    float4 vLightDir; //스포트라이트의 방향

    float fRadius; //점광원 또는 스포트라이트의 거리
    float fAngle;
    UINT32 LightType;
    INT32 padding;
};

struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};

struct tSBufferInfo
{
    UINT32 g_uSBufferCount;
    INT32 iData0;
    INT32 iData1;
    INT32 iData2;
};

struct tRWParticleBuffer
{
    INT32 iSpawnCount; // 스폰 시킬 파티클 개수
    float3 padding;
};





// Particle
struct tParticleTransform
{
    float4 vLocalPos;
    float4 vWorldPos; // 파티클 위치
    float4 vWorldScale; // 파티클 크기
    float4 vColor; // 파티클 색상
    float4 vVelocity; // 파티클 현재 속도
    float4 vForce; // 파티클에 주어진 힘

    float fAge; // 생존 시간
    float fNormalizedAge; // 수명대비 생존시간을 0~1로 정규화 한 값
    float fLifeTime; // 수명
    float fMass; // 질량
    float fScaleFactor; // 추가 크기 배율

    int bActive;
    float2 pad;
};



struct tParticleModule
{
	// 스폰 모듈
    float4 vSpawnColor;
    float4 vSpawnScaleMin;
    float4 vSpawnScaleMax;
    float3 vBoxShapeScale;
    float  fSphereShapeRadius;
    int eSpawnShapeType; // Sphere , Box
    int iSpawnRate;
    int bFollowing;
    float fMinLifeTime;
    float fMaxLifeTime;
    float3 spawnpad;

	// Color Change 모듈
    float4 vStartColor; // 초기 색상
    float4 vEndColor; // 최종 색상

	// Scale Change 모듈
    float fStartScale; // 초기 크기
    float fEndScale; // 최종 크기	

    int iMaxParticleCount;
    int ipad;

	// Add Velocity 모듈
    float4 vVelocityDir;
    int eAddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float fOffsetAngle;
    float fSpeed;
    int addvpad;

	// bDrag 모듈
    float fStartDrag;
    float fEndDrag;
    float2 dragpad;


	// Module Check
    int bSpawn;
    int bColorChange;
    int bScaleChange;
    int bAddVelocity;

    int bDrag;
    float3 modulepad;
};









#endif