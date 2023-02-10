#ifndef SHADER_HEADER_STRUCT
#define SHADER_HEADER_STRUCT

//Header for global values or struct

#define INT32 int
#define UINT32 uint

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
struct tParticle
{
    float4 vLocalPos;
    float4 vWorldPos; // 파티클 위치
    float4 vWorldScale; // 파티클 크기
    float4 vColor; // 파티클 색상
    float4 vVelocity; // 파티클 현재 속도
    float4 vForce; // 파티클에 주어진 힘

    float fAge; // 생존 시간
    float fNomalizedAge; // 수명대비 생존시간을 0~1로 정규화 한 값
    float fLifeTime; // 수명
    float fMass; // 질량
    
    int bActive;
    float3 Padding;
};


struct tParticleModule
{
	// 스폰 모듈
    float4 vSpawnColor;
    float4 vSpawnScale;
    float3 vBoxShapeScale;
    float fSphereShapeRadius;
    INT32 eSpawnShapeType; // Sphere , Box
    INT32 iSpawnRate;
    INT32 bFollowing; // 0 World, 1 Local
    INT32 Padding0;

	// Color Change 모듈
    float4 vStartColor; // 초기 색상
    float4 vEndColor; // 최종 색상

	// Scale Change 모듈
    float4 vStartScale; // 초기 크기
    float4 vEndScale; // 최종 크기	

	// Module Check
    INT32 bSpawn;
    INT32 bColorChange;
    INT32 bScaleChange;
    INT32 Padding1;

    INT32 iMaxParticleCount;
    float3 Padding2;
};









#endif