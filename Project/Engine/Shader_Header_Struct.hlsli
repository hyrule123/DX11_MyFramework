#ifndef SHADER_HEADER_STRUCT
#define SHADER_HEADER_STRUCT

//������� �Ǵ�
//�� �� �̻��� ���̴����� �����ϴ� ����ü��.
//�ϳ��� ���̴����� �����ϰ� ���̴� ����ü�� ��� �ش� ���̴��� ������Ͽ��� �����Ұ�.


//Data Type ����
#ifdef __cplusplus

typedef Vector2     float2;
typedef Vector3     float3;
typedef Vector4     float4;
typedef int         BOOL;

//16 byte
struct UINT16_8 { UINT16 u16[8]; }

//32 byte
struct UINT16_16 { UINT16 u16[16]; }

//64 byte
struct UINT32_16 { UINT32 u32[16]; }

struct INT32_2 { INT32 i32[2]; };
struct INT32_3 { INT32 i32[3]; };
struct INT32_4 { INT32 i32[4]; };

struct UINT32_2 { UINT32 u32[2]; };
struct UINT32_3 { UINT32 u32[3]; };
struct UINT32_4 { UINT32 u32[4]; };

typedef Matrix      MATRIX;

#define SEMANTIC(_Type)

#else

//16 byte
#define UINT16_8 row_major min16uint4x2

//32 byte
#define UINT16_16 row_major min16uint4x4

//64 byte
#define UINT32_16 row_major uint4x4

#define INT32   int
#define INT32_2 int2
#define INT32_3 int3
#define INT32_4 int4

#define UINT32   uint
#define UINT32_2 uint2
#define UINT32_3 uint3
#define UINT32_4 uint4



#define BOOL int
#define MATRIX row_major matrix

#define TRUE 1
#define FALSE 0

#define SEMANTIC(_Type)  : _Type

#endif



//ENUM ����
#ifdef __cplusplus

//C++ : enum class ���·� ������.
#define ENUM_BEGIN(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

//HLSL : namespace ���·� ������.
#define ENUM_BEGIN(_Name, _Type) namespace _Name {
#define ENUM_MEMBER(_Name, _Type, _Val) static const _Type _Name = _Val;
#define ENUM_END };

#endif


struct tMtrlScalarData
{
    INT32 INT_0;
    INT32 INT_1;
    INT32 INT_2;
    INT32 INT_3;
    
	float FLOAT_0;
	float FLOAT_1;
	float FLOAT_2;
	float FLOAT_3;
    
	float2 VEC2_0;
	float2 VEC2_1;
	float2 VEC2_2;
	float2 VEC2_3;

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
    //�ؽ�ó�� ������ �Ǵ��ϱ����� ����
    BOOL bTEX_0;
    BOOL bTEX_1;
    BOOL bTEX_2;
    BOOL bTEX_3;
    BOOL bTEX_4;
    BOOL bTEX_5;
    BOOL bTEX_6;
    BOOL bTEX_7;
};


//      ���� ��ũ��
////C++ : enum���� ��ȯ, 
////HLSL : �Ϲ� Ÿ�Ը����� ��ȯ
#ifdef __cplusplus
#define MTRLDATA_PARAM_SCALAR(_Type) eMTRLDATA_PARAM_SCALAR::##_Type
#else
#define MTRLDATA_PARAM_SCALAR(_Type) _Type
#endif



//==============================
//		���� �� ���� ��Ȳ
//==============================
//C++, HLSL �������� ���



//ī�޶� �������� �������ǹǷ� ī�޶��� ����� ������۸� ���� ���޵ǰ� ����.
//�ʿ��Ҷ��� ī�޶� ����� ���� ���� ��

//g_CBuffer_matCam���� ���
#define MTRL_SCALAR_MAT_VP                     MTRLDATA_PARAM_SCALAR(MAT_0)

//CBuffer �Ǵ� SBuffer���� ���
#define MTRL_SCALAR_MAT_WVP                     MTRLDATA_PARAM_SCALAR(MAT_0)
#define MTRL_SCALAR_MAT_WORLD                   MTRLDATA_PARAM_SCALAR(MAT_1)

//�ڽ��� VP ����� ��� �ִ� ī�޶� ������ۿ����� �ε���
#define MTRL_SCALAR_INT_CAMIDX                  MTRLDATA_PARAM_SCALAR(INT_0)
#define MTRL_SCALAR_STD2D_FLAG                  MTRLDATA_PARAM_SCALAR(INT_1)
#define MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX     MTRLDATA_PARAM_SCALAR(INT_2)

#define MTRL_SCALAR_STD2D_ANIM_UV_LEFTTOP       MTRLDATA_PARAM_SCALAR(VEC2_0)
#define MTRL_SCALAR_STD2D_ANIM_UV_SLICE         MTRLDATA_PARAM_SCALAR(VEC2_1)
#define MTRL_SCALAR_STD2D_ANIM_UV_OFFSET        MTRLDATA_PARAM_SCALAR(VEC2_2)
#define MTRL_SCALAR_STD2D_PIVOT                 MTRLDATA_PARAM_SCALAR(VEC2_3)
#define MTRL_SCALAR_STD2D_COLORKEY              MTRLDATA_PARAM_SCALAR(VEC4_0)

ENUM_BEGIN(eMTRL_SCALAR_STD2D_FLAG, int)
    ENUM_MEMBER(USE_VP, int, 1 << 0) //TRUE == WVP ��ķ� ���� FALSE == VP ��� ���� ����
    ENUM_MEMBER(USE_ANIM, int, 1<<1)
    ENUM_MEMBER(USE_PIVOT, int, 1<<2)
    ENUM_MEMBER(NEED_FLIP_X, int, 1<<3)
    ENUM_MEMBER(USE_COLOR_KEY, int, 1<<4)
ENUM_END


////��ǻƮ���̴�
#define MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_X    MTRLDATA_PARAM_SCALAR(INT_0)
#define MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Y    MTRLDATA_PARAM_SCALAR(INT_1)
#define MTRL_SCALAR_CS_TOTAL_ELEMCOUNT_Z    MTRLDATA_PARAM_SCALAR(INT_2)

//��ǻƮ���̴��� ����ִ� �������ؽ�ó�� �ػ󵵸� ����
#define MTRL_SCALAR_TEXTURE_NOISE_RESOLUTION      MTRLDATA_PARAM_SCALAR(VEC2_0)


//CCS_ParticleUpdate
#define MTRL_SCALAR_OWNER_OBJ_POS MTRLDATA_PARAM_SCALAR(VEC4_0)
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

//�������� �����ϴ� �� ����ü�� �ε��� ��ȣ�� ���� �����ϱ� ���� ����ü(C++������ �����.)
enum class eMTRLDATA_PARAM_SCALAR
{
    INT_0, INT_1, INT_2, INT_3,

    FLOAT_0, FLOAT_1, FLOAT_2, FLOAT_3,

    VEC2_0, VEC2_1, VEC2_2, VEC2_3,

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

//ī��Ʈ�� ���� �ϴ� SBuffer���� �������� ���
struct tSBufferInfo
{
    UINT32 uSBufferCount;
    INT32 iData0;
    INT32 iData1;
    INT32 iData2;
};


//CBuffer���� ����ϹǷ� ���� struct ����� ����
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
    BOOL bModule_ExpandVelocity;    //�ӵ��� ���� ��ƼŬ�� ũ�� ��ȭ��Ű�� ���
    
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
    float4 vStartColor; // �ʱ� ����
    float4 vEndColor; // ���� ����

    
	// Scale Change Module Part
    float fStartScale; // �ʱ� ũ��
    float fEndScale; // ���� ũ��	
	float2 PADDING4;
    
    
    //Rotation Module Part
	float3 vRotRadPerSec;
	float PADDING5;
	float3 vRotRandomRange;     //�� ���� ���̿��� ȸ���ӵ� ����
	float PADDING6;

    
	// Add Velocity Module Part
    float4 vVelocityDir;
    
    INT32 eAddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float fOffsetAngle;
    float fSpeed;
    float PADDING7;

    
	// Drag Module Part : ����ɼ��� �ӵ��� �����ϴ� ȿ��
    float fStartDrag;
    float fEndDrag;
    
    
    // NoiseForce ���
	float fNoiseTerm;
	float fNoiseForce;
        
    // Render ���
	INT32 VelocityAlignment; // 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
	INT32 VelocityScale; // 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
	float vMaxSpeed; // �ִ� ũ�⿡ �����ϴ� �ӷ�
	float PADDING8;
    
    
	float4 vMaxVelocityScale; // �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
    
    
    
    //Gravity ���
	float fGravity;
	float fEnergyLoss;
	float Padding;
    BOOL bHeadingRight;
	
	float4 vOwnerPrevWorldPos;
	float4 vOwnerCurWorldPos;
};


#endif