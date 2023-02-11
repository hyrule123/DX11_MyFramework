#pragma once

//
//
//
//HLSL�� �������ִµ� ����ؾ� �ϴ� ����ü �� ����ü ����
//���⼭ ���� ������ ��� �ݵ�� ���̴����� ���� ������ ��
//
//
//
typedef Vector2 float2;
typedef Vector3 float3;
typedef Vector4 float4;
typedef int BOOL;
typedef Matrix MATRIX;


enum class  eCONST_BUFFER_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL, //b2
	SBUFFER_SHAREDINFO, //b3
	PARTICLE_MODULEDATA,

	END,
};

//��� ���� 'SBUFFERINFO' ������ �ε��� ��ȣ�� �����ϴ� ����ü
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


enum class eMTRLDATA_PARAM_SCALAR
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




#define COLOR_KEY eMTRLDATA_PARAM_SCALAR::VEC4_3

#define CS_TOTAL_ELEMCOUNT_X eMTRLDATA_PARAM_SCALAR::INT_0
#define CS_TOTAL_ELEMCOUNT_Y eMTRLDATA_PARAM_SCALAR::INT_1
#define CS_TOTAL_ELEMCOUNT_Z eMTRLDATA_PARAM_SCALAR::INT_2

enum class  eMTRLDATA_PARAM_TEX
{
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,

	_END,
};

enum class eLIGHT_TYPE
{
	eLIGHT_DIRECTIONAL,	//���籤��
	eLIGHT_POINT,		//������
	eLIGHT_SPOTLIGHT	//����Ʈ����Ʈ
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

//����ȭ ������ ���� �ڿ��� �� ������ۿ� ��Ƽ� ����
struct tSBufferInfo
{
	UINT32 uSBufferCount;
	INT32  iData0;
	INT32  iData1;
	INT32  iData2;
};
extern tSBufferInfo g_arrStructBufferInfo[(UINT)eSBUFFER_SHARED_CBUFFER_IDX::END];


struct tMtrlData
{
	INT32 arrInt[4];
	float arrFloat[4];
	float2 arrV2[4];
	float4 arrV4[4];
	MATRIX arrMat[4];
	INT32 arrbTex[(int)eMTRLDATA_PARAM_TEX::_END];
};

struct tTransform
{
	MATRIX matWorld;
	MATRIX matView;
	MATRIX matProj;
	MATRIX matWVP;
};
extern tTransform g_transform;
extern MATRIX g_matViewProj;

struct tLightColor
{
	float4 vDiffuse;
	float4 vAmbient;
};

struct tLightInfo
{
	tLightColor LightColor;

	float4 vLightWorldPos;

	float4 vLightDir;	//���籤�� �Ǵ� ����Ʈ����Ʈ�� ����

	float fRadius;	//������ �Ǵ� ����Ʈ����Ʈ�� �Ÿ�
	float fAngle;	//����Ʈ����Ʈ�� ��ä�� ����
	UINT LightType;
	INT32 padding;
};

// TileMap
struct tTile
{
	float2 vLeftTop;
	float2 vSlice;
};

//RenderMgr���� ���� ����ü ����
struct tGlobalValue
{
	float2  Resolution;
	float DeltaTime;
	float AccTime;	//���α׷��� �۵� �ð�
};
extern tGlobalValue g_GlobalVal;

struct tDebugShapeInfo
{
	eSHAPE_TYPE	eShape;
	float		fLifeSpan;
	float2		bytepadding;
	MATRIX		matWorld;
	float4		vColor;
};


struct tRWParticleBuffer
{
	INT32		SpawnCount;			// ���� ��ų ��ƼŬ ����
	float3	padding;
};


struct tParticleTransform
{
	float4  vLocalPos;
	float4  vWorldPos; // ��ƼŬ ��ġ
	float4  vWorldScale; // ��ƼŬ ũ��
	float4  vColor; // ��ƼŬ ����
	float4  vVelocity; // ��ƼŬ ���� �ӵ�
	float4  vForce; // ��ƼŬ�� �־��� ��

	float   fAge; // ���� �ð�
	float   fNormalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
	float   fLifeTime; // ����
	float   fMass; // ����
	float   fScaleFactor; // �߰� ũ�� ����

	BOOL     bActive;
	float2  pad;
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
	BOOL bSpawn;
	BOOL bColorChange;
	BOOL bScaleChange;
	BOOL bAddVelocity;

	BOOL bDrag;
	float3 modulepad;
};