#pragma once



struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};
typedef tVertex Vtx;


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


