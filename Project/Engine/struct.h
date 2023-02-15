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

//좌표를 저장하는게 아니라
//각 변의 길이를 저장한다.
union RectLength
{
	struct
	{
		float fLeft;
		float fBottom;
		float fRight;
		float fTop;
	};

	Vec4 LBRT;
};

struct tRectInfo
{
	Vec2 vLBPos;
	Vec2 vRTPos;
	Vec2 vCenterPos;
};



struct tAnim2DFrm
{
	Vec2	LeftTopUV;
	Vec2	SliceUV;
	Vec2	Offset;
	float	fDuration;
};

struct tOBB2D
{
	//방향
	Vec2 m_vAxis[2];

	Vec2 m_vCenterPos;
};

