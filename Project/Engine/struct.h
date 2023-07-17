#pragma once

#include "define.h"

struct tConstBufferClassDesc
{
	int                     iRegisterNum;

	UINT                    uElementSize;
	UINT                    uElementCount;

	UINT                    PipelineStageBindFlag;

	tConstBufferClassDesc()
		: iRegisterNum(-1)
		, uElementSize()
		, uElementCount()
		, PipelineStageBindFlag(UINT8_MAX)  //기본설정: 전체 파이프라인에 바인딩
	{}
};

typedef struct tVertex
{
	Vec3 vPos;
	Vec2 vUV;
	//Vec4 vColor;
} Vtx;


struct tGameEvent
{
	eEVENT_TYPE Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

struct tFSM_Event
{
	UINT uStateID;
	DWORD_PTR srcParam;	//Transition 조건 확인시 체크할 이벤트 파라미터
	DWORD_PTR destParam;//Transition 성공시 새로 바뀐 I_FSM에서 처리할 이벤트 파라미터
	
	tFSM_Event() : uStateID(UINT_MAX), srcParam(), destParam() {}
	tFSM_Event(UINT _uStateID) : uStateID(_uStateID), srcParam(), destParam() {}
	tFSM_Event(UINT _uStateID, DWORD_PTR _srcParam) : uStateID(_uStateID), srcParam(_srcParam), destParam() {}
	tFSM_Event(UINT _uStateID, DWORD_PTR _wParam, DWORD_PTR _destParam) : uStateID(_uStateID), srcParam(_wParam), destParam(_destParam) {}
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

//unordered_map용 비교 함수
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

struct tHasher_String
{
	using hash_type = std::hash<std::string_view>;
	using is_transparent = void;

	std::size_t operator()(const char* str) const { return hash_type{}(str); }
	std::size_t operator()(std::string_view str) const { return hash_type{}(str); }
	std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
};



//map용 비교 함수가 있는 구조체
struct tInstancingKey
{
	//High
	DWORD_PTR pMesh;

	//Low
	DWORD_PTR pMtrl;

	bool operator<(const tInstancingKey& _other) const
	{
		//High 부분에서 값이 차이나면 무조건 클수밖에 없음
		if (pMesh < _other.pMesh)
			return true;

		//High가 같을 경우 Low의 값이 차이나면 True 리턴
		else if (pMesh == _other.pMesh && pMtrl < _other.pMtrl)
			return true;

		//이외의 경우에는 false
		return false;
	}
};

struct tString_Operator
{
	bool operator()(const std::string& _lStr, const std::string& _rStr) const
	{
		return (_lStr < _rStr);
	}

	bool operator()(const std::string& _lStr, const std::string_view& _rStr) const
	{
		return (_lStr < _rStr);
	}

	bool operator()(const std::string_view& _lStr, const std::string& _rStr) const
	{
		return (_lStr < _rStr);
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

