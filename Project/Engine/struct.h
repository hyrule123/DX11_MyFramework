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
	int arrbTex[eTEX_PARAM::TEX_END];
};

struct tTransform
{
	Matrix MatView;
	Matrix MatProj;
};

extern tTransform g_transform;
