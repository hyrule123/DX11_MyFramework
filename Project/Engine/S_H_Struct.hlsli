#ifndef S_H_STRUCT
#define S_H_STRUCT


//상수버퍼 또는
//두 개 이상의 쉐이더에서 참조하는 구조체들.
//하나의 쉐이더에서 고유하게 쓰이는 구조체의 경우 해당 쉐이더의 헤더파일에서 선언할것.


//Data Type 관련
#ifdef __cplusplus

typedef Vector2     float2;
typedef Vector3     float3;
typedef Vector4     float4;
typedef int         BOOL;


struct INT32_2 {
	INT32 x;
	INT32 y;

    INT32_2() : x(), y() {}
	INT32_2(INT32 _xy) : x(_xy), y(_xy) {}
	INT32_2(INT32 _x, INT32 _y) :x(_x), y(_y) {}
};
struct INT32_3 {
	INT32 x;
	INT32 y;
	INT32 z;

    INT32_3() : x(), y(), z() {}
	INT32_3(INT32 _xyz) : x(_xyz), y(_xyz) {}
	INT32_3(INT32 _x, INT32 _y, INT32 _z) : x(_x), y(_y), z(_z) {}
	INT32_3(const INT32_2& _i32_2, INT32 _z) : x(_i32_2.x), y(_i32_2.y), z(_z) {}
};
struct INT32_4 {
	INT32 x;
	INT32 y;
	INT32 z;
	INT32 w;

    INT32_4() : x(), y(), z(), w() {}
	INT32_4(INT32 _xyzw) : x(_xyzw), y(_xyzw), z(_xyzw), w(_xyzw) {}
	INT32_4(INT32 _x, INT32 _y, INT32 _z, INT32 _w) : x(_x), y(_y), z(_z), w(_w) {}
	INT32_4(const INT32_2& _i32_2, INT32 _z, INT32 _w) : x(_i32_2.x), y(_i32_2.y), z(_z), w(_w) {}
	INT32_4(const INT32_2& _i32_2_0, const INT32_2& _i32_2_1) : x(_i32_2_0.x), y(_i32_2_0.y), z(_i32_2_1.x), w(_i32_2_1.y) {}
	INT32_4(const INT32_3& _i32_3, INT32 _w) : x(_i32_3.x), y(_i32_3.y), z(_i32_3.z), w(_w) {}
};

struct UINT32_2 {
	UINT32 x;
	UINT32 y;

    UINT32_2() : x(), y() {}
	UINT32_2(UINT32 _xy) : x(_xy), y(_xy) {}
	UINT32_2(UINT32 _x, UINT32 _y) :x(_x), y(_y) {}
};
struct UINT32_3 {
	UINT32 x;
	UINT32 y;
	UINT32 z;

    UINT32_3() : x(), y(), z() {}
	UINT32_3(UINT32 _xyz) : x(_xyz), y(_xyz) {}
	UINT32_3(UINT32 _x, UINT32 _y, UINT32 _z) : x(_x), y(_y), z(_z) {}
	UINT32_3(const UINT32_2& _i32_2, UINT32 _z) : x(_i32_2.x), y(_i32_2.y), z(_z) {}
};
struct UINT32_4 {
	UINT32 x;
	UINT32 y;
	UINT32 z;
	UINT32 w;

    UINT32_4() : x(), y(), z(), w() {}
	UINT32_4(UINT32 _xyzw) : x(_xyzw), y(_xyzw), z(_xyzw), w(_xyzw) {}
	UINT32_4(UINT32 _x, UINT32 _y, UINT32 _z, UINT32 _w) : x(_x), y(_y), z(_z), w(_w) {}
	UINT32_4(const UINT32_2& _i32_2, UINT32 _z, UINT32 _w) : x(_i32_2.x), y(_i32_2.y), z(_z), w(_w) {}
	UINT32_4(const UINT32_2& _i32_2_0, const UINT32_2& _i32_2_1) : x(_i32_2_0.x), y(_i32_2_0.y), z(_i32_2_1.x), w(_i32_2_1.y) {}
	UINT32_4(const UINT32_3& _i32_3, UINT32 _w) : x(_i32_3.x), y(_i32_3.y), z(_i32_3.z), w(_w) {}
};
struct UINT32_8 { UINT32 u32[8]; };
struct UINT32_16 { UINT32 u32[16]; };

typedef Matrix      MATRIX;

#define SEMANTIC(_Type)

#else

#define FLT_EPSILON 1.192092896e-07F

#define INT32   int
#define INT32_2 int2
#define INT32_3 int3
#define INT32_4 int4

#define UINT32   uint
#define UINT32_2 uint2
#define UINT32_3 uint3
#define UINT32_4 uint4
#define UINT32_8 row_major uint2x4
#define UINT32_16 row_major uint4x4


#define BOOL int
#define MATRIX row_major matrix

#define TRUE 1
#define FALSE 0

#define SEMANTIC(_Type)  : _Type

#endif



//ENUM 관련
#ifdef __cplusplus

//C++ : enum class 형태로 선언함.
#define ENUM_BEGIN(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

//HLSL : namespace 형태로 선언함.
#define ENUM_BEGIN(_Name, _Type) namespace _Name {
#define ENUM_MEMBER(_Name, _Type, _Val) static const _Type _Name = _Val;
#define ENUM_END };

#endif



//UINT64 데이터를 HLSL에 보내고, 이 데이터를 HLSL에서 UINT32 형태로 읽어옴으로써 엔디안 방식을 확인.
//UINT64에 1을 저장하고, 이걸 그대로 HLSL에 올린 뒤 UINT64 형태로 읽어왔을 때 그대로 1이 들어있으면 Little Endian인것.
struct tInitSetting
{
#ifdef __cplusplus
    UINT64 bIsLittleEndian;
    float2 Padding0;
    
    //C++은 int 배열이 4 byte 단위 - HLSL 기준으로 맞춰주기 위함
    //UINT32_4 NotInUse0[4];
    //UINT32_4 NotInUse1[4];
    
    //UINT32_4 NotInUse2[2];
    //UINT32_4 NotInUse3[2];
    
#else
    UINT32 bIsLittleEndian;
	float3 Padding0;
#endif
    
    //HLSL은 int 배열이 16 byte 단위
    UINT32 u8BitPartInU32Pack[4];
    UINT32 u8BitShiftInU32Pack[4];

    UINT32 u16BitPartInU32Pack[2];
    UINT32 u16BitShiftInU32Pack[2];
};

#ifdef __cplusplus
extern tInitSetting g_InitSetting;
#endif


struct tMtrlScalarData
{
    INT32 INT_0;
    INT32 INT_1;
    INT32 INT_2;
    INT32 INT_3;
    
    INT32 INT_4;
    INT32 INT_5;
	float FLOAT_0;
	float FLOAT_1;
    
	float FLOAT_2;
	float FLOAT_3;
	float FLOAT_4;
	float FLOAT_5;
    
	float2 VEC2_0;
	float2 VEC2_1;
	float2 VEC2_2;
	float2 VEC2_3;
    
	float2 VEC2_4;
	float2 VEC2_5;

	float4 VEC4_0;
	float4 VEC4_1;
	float4 VEC4_2;
	float4 VEC4_3;

    MATRIX MAT_0;
    MATRIX MAT_1;
    MATRIX MAT_2;
    MATRIX MAT_3;
};

struct tMtrlTexData
{
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


//      재질 매크로
////C++ : enum으로 전환, 
////HLSL : 일반 타입명으로 전환
#ifdef __cplusplus
//#define MTRLDATA_PARAM_SCALAR(_Type) eMTRLDATA_PARAM_SCALAR::##_Type
#define MTRLDATA_PARAM_SCALAR(Type, Idx) eMTRLDATA_PARAM_SCALAR::##Type##_##Idx
#else
#define MTRLDATA_PARAM_SCALAR(Type, Idx) Type##_##Idx
#endif


//==============================
//		재질 값 예약 현황
//==============================
//C++, HLSL 공용으로 사용




//카메라 기준으로 렌더링되므로 카메라의 행렬은 상수버퍼를 통해 전달되고 있음.
//필요할때만 카메라 행렬을 갖다 쓰면 됨

//g_CBuffer_matCam에서 사용
#define MTRL_SCALAR_MAT_VP                     MTRLDATA_PARAM_SCALAR(MAT, 0)

//CBuffer 또는 SBuffer에서 사용
#define MTRL_SCALAR_MAT_WVP                     MTRLDATA_PARAM_SCALAR(MAT, 0)
#define MTRL_SCALAR_MAT_WORLD                   MTRLDATA_PARAM_SCALAR(MAT, 1)

//자신의 VP 행렬을 들고 있는 카메라 상수버퍼에서의 인덱스
#define MTRL_SCALAR_INT_CAMIDX                  MTRLDATA_PARAM_SCALAR(INT, 0)

//STD2D 기반 쉐이더에서 사용. 플래그 지정
#define MTRL_SCALAR_STD2D_FLAG                  MTRLDATA_PARAM_SCALAR(INT, 1)

//현재 재생하고 있는 애니메이션이 몇 번째 텍스처인지
#define MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX     MTRLDATA_PARAM_SCALAR(INT, 2)

//현재 재생중인 애니메이션이 총 몇 장으로 이루어져 있는지(UINT16 x 2)
#define MTRL_SCALAR_STD2D_ANIM_NUM_ROW_COL      MTRLDATA_PARAM_SCALAR(INT, 3)

#define MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP       MTRLDATA_PARAM_SCALAR(VEC2, 0)
#define MTRL_SCALAR_STD2D_ANIM_UV_SLICE         MTRLDATA_PARAM_SCALAR(VEC2, 1)
#define MTRL_SCALAR_STD2D_ANIM_UV_OFFSET        MTRLDATA_PARAM_SCALAR(VEC2, 2)
#define MTRL_SCALAR_STD2D_PIVOT                 MTRLDATA_PARAM_SCALAR(VEC2, 3)
#define MTRL_SCALAR_STD2D_COLORKEY              MTRLDATA_PARAM_SCALAR(VEC4, 0)


ENUM_BEGIN(eMTRL_SCALAR_STD2D_FLAG, int)
    ENUM_MEMBER(USE_VP, int, 1 << 0) //TRUE == WVP 행렬로 전달 FALSE == VP 행렬 따로 전달
    ENUM_MEMBER(USE_ANIM, int, 1<<1)
    ENUM_MEMBER(USE_PIVOT, int, 1<<2)
    ENUM_MEMBER(NEED_FLIP_X, int, 1<<3)
    ENUM_MEMBER(USE_COLOR_KEY, int, 1<<4)
ENUM_END



////컴퓨트쉐이더
#define MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_X    MTRLDATA_PARAM_SCALAR(INT, 0)
#define MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Y    MTRLDATA_PARAM_SCALAR(INT, 1)
#define MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Z    MTRLDATA_PARAM_SCALAR(INT, 2)

//컴퓨트쉐이더가 들고있는 노이즈텍스처의 해상도를 저장
#define MTRL_SCALAR_TEXTURE_NOISE_RESOLUTION      MTRLDATA_PARAM_SCALAR(VEC2, 0)


//CCS_ParticleUpdate
#define MTRL_SCALAR_OWNER_OBJ_POS MTRLDATA_PARAM_SCALAR(VEC4, 0)
//============================================================================


ENUM_BEGIN(eMTRLDATA_PARAM_TEX, int)
    ENUM_MEMBER(_0, int, 0)
    ENUM_MEMBER(_1, int, 1)
    ENUM_MEMBER(_2, int, 2)
    ENUM_MEMBER(_3, int, 3)
    ENUM_MEMBER(_4, int, 4)
    ENUM_MEMBER(_5, int, 5)
    ENUM_MEMBER(_6, int, 6)
    ENUM_MEMBER(_7, int, 7)
    ENUM_MEMBER(_END, int, 8)
ENUM_END

#ifdef __cplusplus

//재질에서 전달하는 위 구조체를 인덱스 번호를 통해 접근하기 위한 열거체(C++에서만 사용함.)
enum class eMTRLDATA_PARAM_SCALAR
{
    INT_0, INT_1, INT_2, INT_3, INT_4, INT_5,

    FLOAT_0, FLOAT_1, FLOAT_2, FLOAT_3, FLOAT_4, FLOAT_5,

    VEC2_0, VEC2_1, VEC2_2, VEC2_3, VEC2_4, VEC2_5,

    VEC4_0, VEC4_1, VEC4_2, VEC4_3,

    MAT_0, MAT_1, MAT_2, MAT_3,
};

#else


#endif




struct tCamMatrices
{
    MATRIX matView;
    MATRIX matProj;
    MATRIX matVP;
};

ENUM_BEGIN(eCAMERA_INDEX, int)
	ENUM_MEMBER(MAIN, int, 0)
	ENUM_MEMBER(SUB1, int, 1)
	ENUM_MEMBER(SUB2, int, 2)
	ENUM_MEMBER(SUB3, int, 3)
	ENUM_MEMBER(SUB4, int, 4)
	ENUM_MEMBER(EDITOR, int, 5)
	ENUM_MEMBER(END, int, 6)
ENUM_END

#ifdef __cplusplus
extern tCamMatrices g_matCam[(int)eCAMERA_INDEX::END];
#endif



struct tGlobalValue
{
    UINT32_2 u2Res;
	float2 v2Res;
    
    UINT32_2 u2WinSize;
	float2 v2WinSize;
    
	float fDeltaTime;
	float fAccTime;
	float fMapSizeX;
	float fMapSizeY;
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
	BOOL bModule_NoiseForce;
    BOOL bModule_ExpandVelocity;    //속도에 따라 파티클의 크기 변화시키는 모듈
    
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
    float  fSphereShapeRadius;
    
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
	float3 vRotRandomRange;     //이 범위 사이에서 회전속도 랜덤
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


#endif

