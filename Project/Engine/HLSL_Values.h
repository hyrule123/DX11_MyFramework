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
	Matrix		matWorld;
	float4		vColor;
};


struct tRWParticleBuffer
{
	INT32		SpawnCount;			// ���� ��ų ��ƼŬ ����
	float3	padding;
};


struct tParticle
{
	float4  vLocalPos;
	float4  vWorldPos; // ��ƼŬ ��ġ
	float4  vWorldScale; // ��ƼŬ ũ��
	float4  vColor; // ��ƼŬ ����
	float4  vVelocity; // ��ƼŬ ���� �ӵ�
	float4  vForce; // ��ƼŬ�� �־��� ��

	float   Age; // ���� �ð�
	float   NomalizedAge; // ������ �����ð��� 0~1�� ����ȭ �� ��
	float   LifeTime; // ����
	float   Mass; // ����

	int     Active;
	float3  Padding;
};



struct tParticleModule
{
	// ���� ���
	float4  vSpawnColor;
	float4  vSpawnScale;
	float3  vBoxShapeScale;
	float   fSphereShapeRadius;
	INT32     eSpawnShapeType; // Sphere , Box
	INT32     iSpawnRate;
	INT32     bFollowing;          // 0 World, 1 Local
	INT32     Padding0;

	// Color Change ���
	float4  vStartColor; // �ʱ� ����
	float4  vEndColor; // ���� ����

	// Scale Change ���
	float4  vStartScale; // �ʱ� ũ��
	float4  vEndScale; // ���� ũ��	

	// Module Check
	INT32     bSpawn;
	INT32     bColorChange;
	INT32     bScaleChange;
	INT32     Padding1;

	INT32     iMaxParticleCount;
	float3    Padding2;
};