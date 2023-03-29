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




//CheckCollision2D_OBB_OBB 함수에서 이 구조체에 대한 memcpy를 사용하고 있으므로 수정에 주의할 것.
struct tOBB2D
{
	//방향
	Vec2 m_vAxis[2];

	Vec2 m_v2CenterPos;
};


struct tLightHashFunc_UINT32
{
	size_t operator()(const UINT32& _ukey) const
	{
		return static_cast<size_t>(_ukey);
	}
};

struct tLightHashFunc_UINT64
{
	UINT64 operator()(const UINT64& _ukey) const
	{
		return static_cast<UINT64>(_ukey);
	}
};

struct tLightHashFunc_DWORD_PTR
{
	DWORD_PTR operator()(const DWORD_PTR& _ukey) const
	{
		return static_cast<DWORD_PTR>(_ukey);
	}
};



