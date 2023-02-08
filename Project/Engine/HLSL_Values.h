#pragma once

//
//
//
//���� �ٲ� ��� HLSL �ʿ����� ��������־�� �ϴ� ����
//
//
//
enum class  eCONST_BUFFER_TYPE : UINT
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL, //b2
	SBUFFER_SHAREDINFO, //b3
	PARTICLE_MODULEDATA,

	END,
};

//��� ���� 'SBUFFERINFO' ������ �ε��� ��ȣ�� �����ϴ� ����ü
enum class eSBUFFER_SHARED_CBUFFER_IDX : int
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
	LIGHT2D = 8,
	TILE = 9,
	SETCOLOR = 10,
	PARTICLE_INFO = 11,
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

enum class eLIGHT_TYPE : UINT
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
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];
	int arrbTex[eTEX_PARAM::eTEX_END];
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
	Vec4 vDiffuse;
	Vec4 vAmbient;
};

struct tLightInfo
{
	tLightColor LightColor;

	Vec4 vLightWorldPos;

	Vec4 vLightDir;	//���籤�� �Ǵ� ����Ʈ����Ʈ�� ����

	float fRadius;	//������ �Ǵ� ����Ʈ����Ʈ�� �Ÿ�
	float fAngle;	//����Ʈ����Ʈ�� ��ä�� ����
	UINT LightType;
	int padding;
};

// TileMap
struct tTile
{
	Vec2 vLeftTop;
	Vec2 vSlice;
};

//RenderMgr���� ���� ����ü ����
struct tGlobalValue
{
	Vec2  Resolution;
	float DeltaTime;
	float AccTime;	//���α׷��� �۵� �ð�
};
extern tGlobalValue g_GlobalVal;

struct tDebugShapeInfo
{
	eSHAPE_TYPE	eShape;
	float		fLifeSpan;
	Vec2		bytepadding;
	Matrix		matWorld;
	Vec4		vColor;
};


struct tRWParticleBuffer
{
	int		SpawnCount;			// ���� ��ų ��ƼŬ ����
	Vec3	padding;
};


struct tParticle
{
	Vec4	vWorldPos;		// ��ƼŬ ��ġ
	Vec4	vWorldScale;	// ��ƼŬ ũ��
	Vec4	vColor;			// ��ƼŬ ����
	Vec4	vVelocity;		// ��ƼŬ ���� �ӵ�
	Vec4	vForce;			// ��ƼŬ�� �־��� ��

	float   Age;			// ���� �ð�
	float   NomalizedAge;	// ������ �����ð��� 0~1�� ����ȭ �� ��
	float	LifeTime;		// ����
	float	Mass;			// ����
};



struct tParticleModule
{
	// ���� ���
	Vec4    vSpawnColor;

	Vec4	vSpawnScale;

	Vec3	vBoxShapeScale;
	float	fSphereShapeRadius;

	int		SpawnShapeType;		// Sphere , Box
	int		SpawnRate;			// �ʴ� ���� ����
	const Vec2	Padding;

	// Color Change ���
	Vec4	vStartColor;		// �ʱ� ����
	Vec4	vEndColor;			// ���� ����

	// Scale Change ���
	Vec4	vStartScale;		// �ʱ� ũ��
	Vec4	vEndScale;			// ���� ũ��	

	// ���� �ִ�ũ��
	int		iMaxParticleCount;

	// Module Check(���� int 3��¥�� �迭)
	int		bModule_ParticleSpawn;
	int		bModule_ColorChange;
	int		bModule_ScaleChange;
};