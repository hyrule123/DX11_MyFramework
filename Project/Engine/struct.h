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
	Vec2 m_v2Axis[2];
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



//Json 파일에 부동소수점 변수를 int형태로 변환해서 저장하기 위한 union 구조체들
union Pack_float_int
{
	float f;
	int i;

	Pack_float_int(int _i) : i(_i) {};
	Pack_float_int(float _f) : f(_f) {};
};

union Pack_v2_i64
{
	Vec2 v2;
	INT64 i64;

	Pack_v2_i64(Vec2 _v2) : v2(_v2)
	{}

	Pack_v2_i64(INT64 _i64) : i64(_i64)
	{}
};
