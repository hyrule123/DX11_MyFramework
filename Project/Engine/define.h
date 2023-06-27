#pragma once

#include "Precompiled.h"

#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_DOWN(_eKEY) CKeyMgr::GetInst()->GetKeyState(_eKEY) == eKEY_STATE::DOWN		
#define KEY_RELEASE(_eKEY) CKeyMgr::GetInst()->GetKeyState(_eKEY) == eKEY_STATE::RELEASE
#define KEY_PRESSED(_eKEY) CKeyMgr::GetInst()->GetKeyState(_eKEY) == eKEY_STATE::PRESSED

#define DELTA_TIME CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER 32


#define TYPE_INDEX(Type) std::type_index(typeid(Type))

#define FLT_MAX_NEGATIVE -FLT_MAX

#define BITMASK(n) (1 << n)


#define ERROR_MESSAGE(_aStrMessage) MessageBoxA(nullptr, _aStrMessage, NULL, MB_OK); assert(nullptr)


#define ERROR_RUNTIME(_stdRuntimeError) MessageBoxA(nullptr, _stdRuntimeError.what(), nullptr, MB_OK);

#ifdef _DEBUG
#define DEBUG_BREAK DebugBreak()
#else
#define DEBUG_BREAK
#endif

#define STRKEY constexpr inline const char*
#define STRKEY_DECLARE(_type) STRKEY _type = #_type

//Macro for SMALL data sized variable(public)
#define SETTER(_Type_, _Var_, _FuncName_) public: void Set##_FuncName_(_Type_ _##_FuncName_) { _Var_ = _##_FuncName_; }

//Macro for SMALL data sized variable(public)
#define GETTER(_Type_, _Var_, _FuncName_) public: _Type_ Get##_FuncName_() const { return _Var_; }

//Macro for SMALL data sized variable(public)
#define GETTER_SETTER(_Type_, _Var_, _FuncName_) \
public:\
void Set##_FuncName_(_Type_ _##_FuncName_) { _Var_ = _##_FuncName_; }\
_Type_ Get##_FuncName_() const { return _Var_; }

//선언 + Get
#define DECLARE_GETTER(_Type_, _Var_, _FuncName_) \
private: _Type_ _Var_;\
public: _Type_ Get##_FuncName_() const { return _Var_; }

//선언 + Set
#define DECLARE_SETTER(_Type_, _Var_, _FuncName_) \
private: _Type_ _Var_;\
public: void Set##_FuncName_(_Type_ _##_FuncName_) { _Var_ = _##_FuncName_; }

//선언 + Get + Set
#define DECLARE_GETTER_SETTER(_Type_, _Var_, _FuncName_) \
private: _Type_ _Var_;\
public: _Type_ Get##_FuncName_() const { return _Var_; }\
public: void Set##_FuncName_(_Type_ _##_FuncName_) { _Var_ = _##_FuncName_; }

//이 값 이상이면 깊이 프리셋을 설정하지 않았다고 간주한다.
constexpr float DEPTH_PRESET_MAX = 100000.f;

//Global Variable
extern const string g_voidStr;

//새로운 컴포넌트를 만들어줄 경우
//eCOMPONENT_TYPE에 컴포넌트 추가
//components.h에 컴포넌트 뚫어주기
enum class eCOMPONENT_TYPE
{
	// update
	//TRANSFORM,		// 위치, 크기, 회전
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,			// 2차원 광원
	LIGHT3D,			// 3차원 광원
	CAMERA,			// Camera

	// render
	MESH_RENDER,		// 기본적인 렌더링
	PARTICLE_SYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링

	// custom
	SCRIPT_HOLDER,		

	END
};
extern const eCOMPONENT_TYPE g_RenderComIdxStart;
extern const eCOMPONENT_TYPE g_RenderComIdxEnd;





//STRKEY g_eRES_TYPE_strName[(int)eRES_TYPE::END]
//{
//	"MESH",
//	"MESHDATA",
//	"MATERIAL",
//	"TEXTURE",
//	"ANIM2D_ATLAS",
//	"SOUND",
//	"PREFAB",
//	"GRAPHICS_SHADER",
//	"COMPUTE_SHADER"
//};
//
//constexpr std::wstring_view g_eRES_TYPE_lstrName[(int)eRES_TYPE::END]
//{
//	L"MESH",
//	L"MESHDATA",
//	L"MATERIAL",
//	L"TEXTURE",
//	L"ANIM2D_ATLAS",
//	L"SOUND",
//	L"PREFAB",
//	L"GRAPHICS_SHADER",
//	L"COMPUTE_SHADER"
//};

enum class eKEY_STATE
{
	DOWN,
	PRESSED,
	RELEASE,
	NONE, END = NONE,
};

namespace define_Shader
{
	enum class eBLEND_STATE_TYPE
	{
		DEFAULT = 0,
		MASK = 1,
		ALPHA_BLEND = 2,
		ONE_ONE = 3,	//1:1로 섞는 방식 - 컬러키가 검은색인 이미지에 대한 처리
		END
	};

	enum class eDEPTH_STENCIL_TYPE
	{
		LESS = 0,
		LESS_EQUAL = 1,
		GREATER = 2,
		GREATER_EQUAL = 3,
		NO_WRITE = 4,			//깊이버퍼에 깊이를 기록하지 않음.
		NO_TEST_NO_WRITE = 5,	//깊이 테스트랄 거지치 않고 무조건 픽셀에 색상을 입력. 깊이 버퍼에도 기록하지 않음.
		END
	};

	enum class eRASTERIZER_TYPE
	{
		CULL_BACK = 0,
		CULL_FRONT = 1,
		CULL_NONE = 2,
		WIRE_FRAME = 3,
		END
	};


	enum class eSHADER_DOMAIN
	{
		_UNDEFINED = -1,
		_OPAQUE = 0,
		_MASK = 1,
		_TRANSPARENT = 2,
		_POST_PROCESS = 3,
		_UI = 4,
		_END
	};

	namespace eSHADER_PIPELINE_STAGE
	{
		enum FLAG : UINT
		{
			__NONE = 0,
			__VERTEX = BITMASK(0),
			__HULL = BITMASK(1),
			__DOMAIN = BITMASK(2),
			__GEOMETRY = BITMASK(3),
			__PIXEL = BITMASK(4),
			__COMPUTE = BITMASK(5),

			__ALL = UINT8_MAX
		};
	}

	//
	enum D3D_PRIMITIVE_TOPOLOGY;


	enum class eSHADER_TYPE
	{
		__VERTEX = 0,
		__HULL = 1,
		__DOMAIN = 2,
		__GEOMETRY = 3,
		__PIXEL = 4,
		END
	};



}


enum class eAXIS2D : UINT
{
	X,
	Y,
	END
};


enum class eAXIS3D : UINT
{
	X,
	Y,
	Z,
	END
};




enum class eAXIS4D : UINT
{
	X,
	Y,
	Z,
	W,
	END
};



//직관적인 방향
enum class eDIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END
};



enum class ePROJ_TYPE
{
	ORTHOGRAPHY,
	PERSPECTIVE
};



enum class eEVENT_TYPE
{
	SPAWN_OBJECT,
	DELETE_OBJECT,
	ADD_CHILD,
	DELETE_RESOURCE,
	LEVEL_CHANGE,
	REMOVE_COMPONENT
};

enum class eCOLLIDER_TYPE_2D
{
	RECT,
	CIRCLE,
	OBB,
	
	POINT,
	END
};



enum class eCOLLIDER_TYPE_3D
{
	LINE
};

enum class eDIMENSION_TYPE : UINT
{
	_2D = 2u,
	_3D = 3u
};

enum class eSTRUCT_BUFFER_TYPE
{
	READ_ONLY,  //SRV ONLY
	READ_WRITE  //SRV + UAV(Compute Shader)
} typedef eSTRUCT_BUFFER_BIND_TYPE;

//현재 바인딩되어 있는 뷰. 플래그로도 사용 가능
enum class eCURRENT_BOUND_VIEW
{
	NONE,
	SRV,
	UAV,
	RTV, 
	DSV,
};



enum class eLEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP
};

enum class eANIM_PLAYMODE
{
	NONE,
	DISABLE_AFTER_PLAY,
	NORMAL_LOOP,
	ZIG_ZAG
};



#include "S_H_Struct.hlsli"
