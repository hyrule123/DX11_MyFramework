#pragma once



typedef struct tVertex
{
	Vec3 vPos;
	Vec2 vUV;
	//Vec4 vColor;
} Vtx;


struct tEvent
{
	eEVENT_TYPE Type;
	DWORD_PTR	lParam;
	DWORD_PTR	rParam;
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

struct tAnim2DFrm
{
	Vec2	LeftTopUV;
	Vec2	SliceUV;
	Vec2	Offset;
	float	fDuration;
};

