#ifndef SHADER_HEADER_STRUCT
#define SHADER_HEADER_STRUCT





//Header for global values or struct

#ifdef __cplusplus

typedef Vector2     float2;
typedef Vector3     float3;
typedef Vector4     float4;
typedef int         BOOL;
typedef Matrix      MATRIX;

//C++ : enum class ���·� ������.
#define ENUM_START(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

#define INT32 int
#define UINT32 uint
#define BOOL int
#define MATRIX row_major matrix

//HLSL : namespace ���·� ������.
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
#define MTRL_PARAM_SCALAR(_Type) eMTRLDATA_PARAM_SCALAR::##_Type
#else
#define MTRL_PARAM_SCALAR(_Type) g_CBuffer_MtrlData.##_Type
#endif

//C++, HLSL �������� ���
#define COLOR_KEY               MTRL_PARAM_SCALAR(VEC4_3)
#define CS_TOTAL_ELEMCOUNT_X    MTRL_PARAM_SCALAR(INT_0)
#define CS_TOTAL_ELEMCOUNT_Y    MTRL_PARAM_SCALAR(INT_1)
#define CS_TOTAL_ELEMCOUNT_Z    MTRL_PARAM_SCALAR(INT_2)


//CCS_ParticleUpdate
#define OWNER_OBJ_POS MTRL_PARAM_SCALAR(VEC4_0)
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





struct tLightColor
{
    float4 vDiffuse;
    float4 vAmbient;
};

struct tLightInfo
{
    tLightColor LightColor;

    float4 vLightWorldPos;

    float4 vLightDir; //����Ʈ����Ʈ�� ����

    float fRadius; //������ �Ǵ� ����Ʈ����Ʈ�� �Ÿ�
    float fAngle;
    INT32 LightType;   //�Ʒ� ENUM���� �������.
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
    INT32 iSpawnCount; // ���� ��ų ��ƼŬ ����
    float3 padding;
};





// Particle
struct tParticleTransform
{
    float4 vLocalPos;
    float4 vWorldPos; // ��ƼŬ ��ġ
    float4 vWorldScale; // ��ƼŬ ũ��
    float4 vColor; // ��ƼŬ ����
    float4 vVelocity; // ��ƼŬ ���� �ӵ�
    float4 vForce; // ��ƼŬ�� �־��� ��

    float fAge; // ���� �ð�
    float fNormalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
    float fLifeTime; // ����
    float fMass; // ����
    float fScaleFactor; // �߰� ũ�� ����

    int bActive;
    float2 pad;
};



struct tParticleModule
{
	// ���� ���
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

	// Color Change ���
    float4 vStartColor; // �ʱ� ����
    float4 vEndColor; // ���� ����

	// Scale Change ���
    float fStartScale; // �ʱ� ũ��
    float fEndScale; // ���� ũ��	

    int iMaxParticleCount;
    int ipad;

	// Add Velocity ���
    float4 vVelocityDir;
    int eAddVelocityType; // 0 : From Center, 1 : Fixed Direction	
    float fOffsetAngle;
    float fSpeed;
    int addvpad;

	// bDrag ���
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