#pragma once

//
//
//
//���� �ٲ� ��� HLSL �ʿ����� ��������־�� �ϴ� ����
//
//
//


enum eCONST_BUFFER_TYPE : UINT
{
	eCONST_BUFFER_TRANSFORM,	// b0
	eCONST_BUFFER_MATERIAL,	// b1
	eCONST_BUFFER_GLOBAL, //b2
	eCONST_BUFFER_SBUFFERINFO, //b3

	eCONST_BUFFER_END,
};

//��� ���� 'SBUFFERINFO' ������ �ε��� ��ȣ�� �����ϴ� ����ü
enum eSBUFFER_SHARED_CBUFFER_IDX : UINT
{
	LIGHT2D,
	TILE,
	END
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






//=====================================================
//                 STRUCT
//==========================================

//����ȭ ������ ���� �ڿ��� �� ������ۿ� ��Ƽ� ����
struct tSBufferInfo
{
	UINT32 uSBufferCount;
	Vec3 Padding;
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