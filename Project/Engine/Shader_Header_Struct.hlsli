#ifndef SHADER_HEADER_STRUCT
#define SHADER_HEADER_STRUCT

//상수버퍼 또는
//두 개 이상의 쉐이더에서 참조하는 구조체들.
//하나의 쉐이더에서 고유하게 쓰이는 구조체의 경우 해당 쉐이더의 헤더파일에서 선언할것.


//Data Type 관련
#ifdef __cplusplus

typedef Vector2     float2;
typedef Vector3     float3;
typedef Vector4     float4;
typedef int         BOOL;
typedef Matrix      MATRIX;

#define SEMANTIC(_Type)

#else

#define INT32 int
#define UINT32 uint
#define BOOL int
#define MATRIX row_major matrix

#define TRUE 1
#define FALSE 0

#define SEMANTIC(_Type)  : _Type

#endif



//ENUM 관련
#ifdef __cplusplus

//C++ : enum class 형태로 선언함.
#define ENUM_START(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

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
#define MTRLDATA_PARAM_SCALAR(_Type) eMTRLDATA_PARAM_SCALAR::##_Type
#else
#define MTRLDATA_PARAM_SCALAR(_Type) g_CBuffer_MtrlData.##_Type
#endif

//C++, HLSL 공용으로 사용
#define COLOR_KEY               MTRLDATA_PARAM_SCALAR(VEC4_3)
#define CS_TOTAL_ELEMCOUNT_X    MTRLDATA_PARAM_SCALAR(INT_0)
#define CS_TOTAL_ELEMCOUNT_Y    MTRLDATA_PARAM_SCALAR(INT_1)
#define CS_TOTAL_ELEMCOUNT_Z    MTRLDATA_PARAM_SCALAR(INT_2)

//컴퓨트쉐이더가 들고있는 노이즈텍스처의 해상도를 저장
#define TEXTURE_NOISE_RESOLUTION      MTRLDATA_PARAM_SCALAR(VEC2_0)


//CCS_ParticleUpdate
#define OWNER_OBJ_POS MTRLDATA_PARAM_SCALAR(VEC4_0)
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



struct tDebugShapeInfo
{
	int eShapeType;
	float fLifeSpan;
	float2 bytepadding;
	MATRIX matWorld;
	float4 vColor;
};

//카운트를 세야 하는 SBuffer에서 공용으로 사용
struct tSBufferInfo
{
    UINT32 uSBufferCount;
    INT32 iData0;
    INT32 iData1;
    INT32 iData2;
};


//CBuffer에서 사용하므로 공용 struct 헤더에 선언
struct tParticleModule
{
	//Module Switch + Basic Info
    BOOL bModule_Spawn;
    BOOL bModule_ColorChange;
	BOOL bModule_ScaleChange;
    BOOL bModule_Rotation;
	BOOL bModule_AddVelocity;
    
	BOOL bModule_Drag;
	int iMaxParticleCount;
	float modulepad;
    
    
    //Spawn Module Part
	int eSpawnShapeType; // Sphere , Box
	int iSpawnRate;
	int bFollowing;
	float SpawnPadding1;
    
    float4 vSpawnColor;
    float4 vSpawnScaleMin;
    float4 vSpawnScaleMax;
    
    float3 vBoxShapeScale;
    float  fSphereShapeRadius;
    
    float fMinLifeTime;
    float fMaxLifeTime;
	float2 SpawnPadding2;

    
	//Color Change Module Part
    float4 vStartColor; // 초기 색상
    float4 vEndColor; // 최종 색상

    
	// Scale Change Module Part
    float fStartScale; // 초기 크기
    float fEndScale; // 최종 크기	
	float2 ScaleChangePadding;
    
    
    //Rotation Module Part
	float3 vRotRadPerSec;
	float RotModulePadding1;
	float3 vRotRandomRange;     //이 범위 사이에서 회전속도 랜덤
	float RotModulePadding2;

    
	// Add Velocity Module Part
    float4 vVelocityDir;
    
    int eAddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float fOffsetAngle;
    float fSpeed;
    int AddVelocityPadding;

    
	// Drag Module Part : 진행될수록 속도가 감소하는 효과
    float fStartDrag;
    float fEndDrag;
    float2 dragpad;
};




#endif