#ifndef SHADER_HEADER_STRUCT
#define SHADER_HEADER_STRUCT





//Header for global values or struct

#ifdef __cplusplus

typedef Vector2     float2;
typedef Vector3     float3;
typedef Vector4     float4;
typedef int         BOOL;
typedef Matrix      MATRIX;

//C++ : enum class 형태로 선언함.
#define ENUM_START(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

#define INT32 int
#define UINT32 uint
#define BOOL int
#define MATRIX row_major matrix

//HLSL : namespace 형태로 선언함.
#define ENUM_START(_Name, _Type) namespace _Name {
#define ENUM_MEMBER(_Name, _Type, _Val) static const _Type _Name = _Val;
#define ENUM_END };
#endif


struct tMtrlData
{
    int     INT_0;
    int     INT_1;
    int     INT_2;
    int     INT_3;
    
    float   FLOAT_0;
    float   FLOAT_1;
    float   FLOAT_2;
    float   FLOAT_3;
    
    float2  VEC2_0;
    float2  VEC2_1;
    float2  VEC2_2;
    float2  VEC2_3;

    float4  VEC4_0;
    float4  VEC4_1;
    float4  VEC4_2;
    float4  VEC4_3;

    MATRIX  MAT_0;
    MATRIX  MAT_1;
    MATRIX  MAT_2;
    MATRIX  MAT_3;
    
    
    //텍스처의 유무를 판단하기위한 변수
    BOOL bTEX_0;
    BOOL bTEX_1;
    BOOL bTEX_2;
    BOOL bTEX_3;
    BOOL bTEX_4;
    BOOL bTEX_5;
    BOOL bTEX_6;
    BOOL bTEX_7;
};


//==============================
//		재질 값 예약 현황
//==============================
//C++ : enum으로 전환, 
//HLSL : 일반 타입명으로 전환
#ifdef __cplusplus
#define MTRL_PARAM_SCALAR(_Type) eMTRLDATA_PARAM_SCALAR::##_Type
#else
#define MTRL_PARAM_SCALAR(_Type) g_CBuffer_MtrlData.##_Type
#endif

//C++, HLSL 공용으로 사용
#define COLOR_KEY               MTRL_PARAM_SCALAR(VEC4_3)
#define CS_TOTAL_ELEMCOUNT_X    MTRL_PARAM_SCALAR(INT_0)
#define CS_TOTAL_ELEMCOUNT_Y    MTRL_PARAM_SCALAR(INT_1)
#define CS_TOTAL_ELEMCOUNT_Z    MTRL_PARAM_SCALAR(INT_2)

//컴퓨트쉐이더가 들고있는 노이즈텍스처의 해상도를 저장
#define TEXTURE_NOISE_RESOLUTION      MTRL_PARAM_SCALAR(VEC2_0)


//CCS_ParticleUpdate
#define OWNER_OBJ_POS MTRL_PARAM_SCALAR(VEC4_0)
//============================================================================


#ifdef __cplusplus

//재질에서 전달하는 위 구조체를 인덱스 번호를 통해 접근하기 위한 열거체(C++에서만 사용함.)
enum class eMTRLDATA_PARAM_SCALAR
{
    INT_0, INT_1, INT_2, INT_3,

    FLOAT_0, FLOAT_1, FLOAT_2, FLOAT_3,

    VEC2_0, VEC2_1, VEC2_2, VEC2_3,

    VEC4_0, VEC4_1, VEC4_2, VEC4_3,

    MAT_0, MAT_1, MAT_2, MAT_3,
};

//C++ 재질에서 예약해놓은 버퍼 번호
enum class eMTRLDATA_PARAM_TEX
{
	_0, _1, _2, _3, _4, _5, _6, _7, _END
};

#else


#endif


struct tTransform
{
    MATRIX matWorld;
    MATRIX matView;
    MATRIX matProj;
    MATRIX matWVP;
};

#ifdef __cplusplus
extern tTransform g_Transform;
extern MATRIX       g_matViewProj;
#endif



struct tGlobalValue
{
    float2 vResolution;
    float fDeltaTime;
    float fAccTime;
};
#ifdef __cplusplus
extern tGlobalValue g_GlobalVal;
#endif





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
    INT32 LightType;   //아래 ENUM값이 들어있음.
    INT32 padding;
};

ENUM_START(eLIGHT_TYPE, int)
ENUM_MEMBER(DIRECTIONAL, int, 0)
ENUM_MEMBER(POINT, int, 1)
ENUM_MEMBER(SPOTLIGHT, int, 2)
ENUM_END



struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};


struct tDebugShapeInfo
{
	int eShapeType;
	float fLifeSpan;
	float2 bytepadding;
	MATRIX matWorld;
	float4 vColor;
};



struct tSBufferInfo
{
    UINT32 uSBufferCount;
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

	// bModule_Drag 모듈
    float fStartDrag;
    float fEndDrag;
    float2 dragpad;


	// Module Check
    int bModule_Spawn;
    int bModule_ColorChange;
    int bModule_ScaleChange;
    int bModule_AddVelocity;

    int bModule_Drag;
    float3 modulepad;
};




#endif