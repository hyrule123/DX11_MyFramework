#pragma once

//
//
//
//HLSL로 전달해주는데 사용해야 하는 열거체 및 구조체 모음
//여기서 값을 변경할 경우 반드시 쉐이더쪽의 값도 수정할 것
//
//
//
typedef Vector2 float2;
typedef Vector3 float3;
typedef Vector4 float4;


enum class  eCONST_BUFFER_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL, //b2
	SBUFFER_SHAREDINFO, //b3
	PARTICLE_MODULEDATA,

	END,
};

//상수 버퍼 'SBUFFERINFO' 내부의 인덱스 번호를 지정하는 열거체
enum class eSBUFFER_SHARED_CBUFFER_IDX
{
	NONE = -1,
	LIGHT2D,
	TILE,
	SETCOLOR,
	PARTICLE,
	END
};

enum class eSRV_REGISTER_IDX
{
	NONE = -1,
	LIGHT2D = 12,
	TILE = 13,
	SETCOLOR = 14,
	PARTICLE_INFO = 15,
	NOISE_TEXTURE = 16
};

enum class eUAV_REGISTER_IDX
{
	NONE = -1,
	SETCOLOR_TEXTURE,
	SETCOLOR_SBUFFER,
	PARTICLE_SBUFFER,
	PARTICLE_SBUFFER_SHARED
};


enum eSCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

};
#define COLOR_KEY VEC4_3

#define CS_TotalCountX eSCALAR_PARAM::INT_0
#define CS_TotalCountY eSCALAR_PARAM::INT_1
#define CS_TotalCountZ eSCALAR_PARAM::INT_2

enum eTEX_PARAM
{
	eTEX_0,
	eTEX_1,
	eTEX_2,
	eTEX_3,
	eTEX_4,
	eTEX_5,
	eTEX_6,
	eTEX_7,

	eTEX_END,
};

enum class eLIGHT_TYPE
{
	eLIGHT_DIRECTIONAL,	//직사광선
	eLIGHT_POINT,		//점광원
	eLIGHT_SPOTLIGHT	//스포트라이트
};


enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,
	COLOR_CHANGE,
	SCALE_CHANGE,

	END
};



//=====================================================
//                 STRUCT
//==========================================

//구조화 버퍼의 공유 자원을 이 상수버퍼에 모아서 전달
struct tSBufferInfo
{
	UINT32 uSBufferCount;
	INT32  iData0;
	INT32  iData1;
	INT32  iData2;
};
extern tSBufferInfo g_arrStructBufferInfo[(UINT)eSBUFFER_SHARED_CBUFFER_IDX::END];


struct tMtrlConst
{
	INT32 arrInt[4];
	float arrFloat[4];
	float2 arrV2[4];
	float4 arrV4[4];
	Matrix arrMat[4];
	INT32 arrbTex[eTEX_PARAM::eTEX_END];
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	Matrix matWVP;
};
extern tTransform g_transform;
extern Matrix g_matViewProj;

struct tLightColor
{
	float4 vDiffuse;
	float4 vAmbient;
};

struct tLightInfo
{
	tLightColor LightColor;

	float4 vLightWorldPos;

	float4 vLightDir;	//직사광선 또는 스포트라이트의 방향

	float fRadius;	//점광원 또는 스포트라이트의 거리
	float fAngle;	//스포트라이트의 부채꼴 각도
	UINT LightType;
	INT32 padding;
};

// TileMap
struct tTile
{
	float2 vLeftTop;
	float2 vSlice;
};

//RenderMgr에서 보낼 구조체 변수
struct tGlobalValue
{
	float2  Resolution;
	float DeltaTime;
	float AccTime;	//프로그래밍 작동 시간
};
extern tGlobalValue g_GlobalVal;

struct tDebugShapeInfo
{
	eSHAPE_TYPE	eShape;
	float		fLifeSpan;
	float2		bytepadding;
	Matrix		matWorld;
	float4		vColor;
};


struct tRWParticleBuffer
{
	INT32		SpawnCount;			// 스폰 시킬 파티클 개수
	float3	padding;
};


struct tParticle
{
	float4  vLocalPos;
	float4  vWorldPos; // 파티클 위치
	float4  vWorldScale; // 파티클 크기
	float4  vColor; // 파티클 색상
	float4  vVelocity; // 파티클 현재 속도
	float4  vForce; // 파티클에 주어진 힘

	float   Age; // 생존 시간
	float   NomalizedAge; // 수명대비 생존시간을 0~1로 정규화 한 값
	float   LifeTime; // 수명
	float   Mass; // 질량

	int     Active;
	float3  Padding;
};



struct tParticleModule
{
	// 스폰 모듈
	float4  vSpawnColor;
	float4  vSpawnScale;
	float3  vBoxShapeScale;
	float   fSphereShapeRadius;
	INT32     eSpawnShapeType; // Sphere , Box
	INT32     iSpawnRate;
	INT32     bFollowing;          // 0 World, 1 Local
	INT32     Padding0;

	// Color Change 모듈
	float4  vStartColor; // 초기 색상
	float4  vEndColor; // 최종 색상

	// Scale Change 모듈
	float4  vStartScale; // 초기 크기
	float4  vEndScale; // 최종 크기	

	// Module Check
	INT32     bSpawn;
	INT32     bColorChange;
	INT32     bScaleChange;
	INT32     Padding1;

	INT32     iMaxParticleCount;
	float3    Padding2;
};