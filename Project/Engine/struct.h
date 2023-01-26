#pragma once

struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

typedef tVertex Vtx;


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

struct tEvent
{
	eEVENT_TYPE Type;
	DWORD_PTR	lParam;
	DWORD_PTR	rParam;
};

struct tDebugShapeInfo
{
	eSHAPE_TYPE	eShape;
	float		fLifeSpan;
	Vec2		bytepadding;
	Matrix		matWorld;
	Vec4		vColor;
};

struct tSquareInfo
{
	Vec2 LB;
	float Size;
};

union tRectInfo
{
	struct 
	{
		Vec2 LB;
		Vec2 RT;
	};
	Vec4 LBRT;
};

typedef Vector4 tRectLBRT;

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



//RenderMgr���� ���� ����ü ����
struct tGlobalValue
{
	Vec2  Resolution;
	float DeltaTime;
	float AccTime;	//���α׷��� �۵� �ð�

	UINT  Light2DCount;
	UINT  Light3DCount;
	Vec2  Padding;
};
extern tGlobalValue g_GlobalVal;