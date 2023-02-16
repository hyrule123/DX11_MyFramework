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

struct INT32_2 { INT32 i[2]; };
struct INT32_3 { INT32 i[3]; };
struct INT32_4 { INT32 i[4]; };

struct UINT32_2 { UINT32 u[2]; };
struct UINT32_3 { UINT32 u[3]; };
struct UINT32_4 { UINT32 u[4]; };

typedef Matrix      MATRIX;

#define SEMANTIC(_Type)

#else

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
#define ENUM_START(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

//HLSL : namespace ���·� ������.
#define ENUM_START(_Name, _Type) namespace _Name {
#define ENUM_MEMBER(_Name, _Type, _Val) static const _Type _Name = _Val;
#define ENUM_END };

#endif


struct tMtrlData
{
    INT32   INT_0;
    INT32   INT_1;
    INT32   INT_2;
    INT32   INT_3;
    
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


//==============================
//		���� �� ���� ��Ȳ
//==============================
//C++ : enum���� ��ȯ, 
//HLSL : �Ϲ� Ÿ�Ը����� ��ȯ
#ifdef __cplusplus
#define MTRLDATA_PARAM_SCALAR(_Type) eMTRLDATA_PARAM_SCALAR::##_Type
#else
#define MTRLDATA_PARAM_SCALAR(_Type) g_CBuffer_MtrlData.##_Type
#endif

//C++, HLSL �������� ���
#define COLOR_KEY               MTRLDATA_PARAM_SCALAR(VEC4_3)
#define CS_TOTAL_ELEMCOUNT_X    MTRLDATA_PARAM_SCALAR(INT_0)
#define CS_TOTAL_ELEMCOUNT_Y    MTRLDATA_PARAM_SCALAR(INT_1)
#define CS_TOTAL_ELEMCOUNT_Z    MTRLDATA_PARAM_SCALAR(INT_2)

//��ǻƮ���̴��� ����ִ� �������ؽ�ó�� �ػ󵵸� ����
#define TEXTURE_NOISE_RESOLUTION      MTRLDATA_PARAM_SCALAR(VEC2_0)


//CCS_ParticleUpdate
#define OWNER_OBJ_POS MTRLDATA_PARAM_SCALAR(VEC4_0)
//============================================================================


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

//C++ �������� �����س��� ���� ��ȣ
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
    
	INT32 iMaxParticleCount;
	float3 PADDING1;
    
    
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
};
#endif