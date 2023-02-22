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

//��ǥ�� �����ϴ°� �ƴ϶�
//�� ���� ���̸� �����Ѵ�.
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





struct tOBB2D
{
	//����
	Vec2 m_vAxis[2];

	Vec2 m_vCenterPos;
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