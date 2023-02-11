#ifndef SHADER_HEADER_STRUCT
#define SHADER_HEADER_STRUCT

//Header for global values or struct

#ifdef __cplusplus

typedef Vector2     float2;
typedef Vector3     float3;
typedef Vector4     float4;
typedef int         BOOL;
typedef Matrix      MATRIX;

#else

#define INT32 int
#define UINT32 uint
#define BOOL int
#define MATRIX row_major matrix

#endif



//enum
//class eLIGHT_TYPE
//{
//	eLIGHT_DIRECTIONAL,	//���籤��
//	eLIGHT_POINT,		//������
//	eLIGHT_SPOTLIGHT	//����Ʈ����Ʈ
//};


//enum
//class PARTICLE_MODULE
//{
//	PARTICLE_SPAWN,
//	COLOR_CHANGE,
//	SCALE_CHANGE,

//	END
//};


struct tMtrlData
{
    int     int_0;
    int     int_1;
    int     int_2;
    int     int_3;
    
    float   float_0;
    float   float_1;
    float   float_2;
    float   float_3;
    
    float2  vec2_0;
    float2  vec2_1;
    float2  vec2_2;
    float2  vec2_3;

    float4  vec4_0;
    float4  vec4_1;
    float4  vec4_2;
    float4  vec4_3;

    MATRIX  mat_0;
    MATRIX  mat_1;
    MATRIX  mat_2;
    MATRIX  mat_3;
    
    
    //�ؽ�ó�� ������ �Ǵ��ϱ����� ����
    BOOL btex_0;
    BOOL btex_1;
    BOOL btex_2;
    BOOL btex_3;
    BOOL btex_4;
    BOOL btex_5;
    BOOL btex_6;
    BOOL btex_7;
};


//C++ : enum���� ��ȯ, 
//HLSL : �Ϲ� Ÿ�Ը����� ��ȯ
#ifdef __cplusplus
#define MTRL_PARAM_SCALAR(Type) eMTRLDATA_PARAM_SCALAR::##Type
#else
#define MTRL_PARAM_SCALAR(Type) Type
#endif

#define COLOR_KEY               MTRL_PARAM_SCALAR(vec4_3)
#define CS_TOTAL_ELEMCOUNT_X    MTRL_PARAM_SCALAR(int_0)
#define CS_TOTAL_ELEMCOUNT_Y    MTRL_PARAM_SCALAR(int_1)
#define CS_TOTAL_ELEMCOUNT_Z    MTRL_PARAM_SCALAR(int_3)


#ifdef __cplusplus

//�������� �����ϴ� �� ����ü�� �ε��� ��ȣ�� ���� �����ϱ� ���� ����ü
enum class eMTRLDATA_PARAM_SCALAR
{
    int_0, int_1, int_2, int_3,

    float_0, float_1, float_2, float_3,

    vec2_0, vec2_1, vec2_2, vec2_3,

    vec4_0, vec4_1, vec4_2, vec4_3,

    mat_0, mat_1, mat_2, mat_3,
};

//C++ �������� �����س��� ���� ��ȣ
enum class eMTRLDATA_PARAM_TEX
{
	_0, _1, _2, _3, _4, _5, _6, _7
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

    float4 vLightDir; //����Ʈ����Ʈ�� ����

    float fRadius; //������ �Ǵ� ����Ʈ����Ʈ�� �Ÿ�
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