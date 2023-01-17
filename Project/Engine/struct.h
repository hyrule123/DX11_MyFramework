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
	Matrix matViewProj;
};

extern tTransform g_transform;

struct tEvent
{
	eEVENT_TYPE Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
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
	float LB_X;
	float LB_Y;
	float Size;
};

struct tRectInfo
{
	Vec2 LB;
	Vec2 Size;
};

class CCollider2D;
struct tColliderPartInfo
{
	CCollider2D* pCol;
	tRectInfo    RectInfo;
	bool		 bFinal;
};
