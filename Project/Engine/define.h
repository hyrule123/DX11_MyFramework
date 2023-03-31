#pragma once

#include "Precompiled.h"

#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(_eKEY) CKeyMgr::GetInst()->GetKeyState(_eKEY) == eKEY_STATE::TAP		
#define KEY_RELEASE(_eKEY) CKeyMgr::GetInst()->GetKeyState(_eKEY) == eKEY_STATE::RELEASE
#define KEY_PRESSED(_eKEY) CKeyMgr::GetInst()->GetKeyState(_eKEY) == eKEY_STATE::PRESSED

#define DELTA_TIME CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER 32


#define TYPE_INDEX(Type) std::type_index(typeid(Type))

#define FLT_MAX_NEG -FLT_MAX

#define BITMASK(n) (1 << n)
#define ERROR_MESSAGE(_aStrMessage) MessageBoxA(nullptr, #_aStrMessage, NULL, MB_OK)
#define ERROR_RUNTIME(_stdRuntimeError) MessageBoxA(nullptr, _stdRuntimeError.what(), nullptr, MB_OK);



extern const string g_voidStr;

//새로운 컴포넌트를 만들어줄 경우
//eCOMPONENT_TYPE에 컴포넌트 추가
//components.h에 컴포넌트 뚫어주기
enum class eCOMPONENT_TYPE
{
	// update
	TRANSFORM,		// 위치, 크기, 회전
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
	FSM,

	END
};
extern const eCOMPONENT_TYPE g_RenderComIdxStart;
extern const eCOMPONENT_TYPE g_RenderComIdxEnd;





//constexpr std::string_view g_eRES_TYPE_strName[(int)eRES_TYPE::END]
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
	TAP,
	PRESSED,
	RELEASE,
	NONE, END = NONE,

};


//================
//	Shader Flags
//================
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

		__ALL  = UINT8_MAX
	};
}

//
enum D3D_PRIMITIVE_TOPOLOGY;


enum class eSHADER_TYPE
{
	__VERTEX,
	__HULL,
	__DOMAIN,
	__GEOMETRY,
	__PIXEL,
	END
};

enum class eRASTERIZER_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END
};

enum class eDEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			//깊이버퍼에 깊이를 기록하지 않음.
	NO_TEST_NO_WRITE,	//깊이 테스트랄 거지치 않고 무조건 픽셀에 색상을 입력. 깊이 버퍼에도 기록하지 않음.
	END
};

enum class eBLEND_STATE_TYPE
{
	DEFAULT,
	MASK,
	ALPHA_BLEND,
	ONE_ONE,	//1:1로 섞는 방식 - 컬러키가 검은색인 이미지에 대한 처리
	END
};

enum class eSHADER_DOMAIN
{
	_UNDEFINED = -1,
	_OPAQUE,
	_MASK,
	_TRANSPARENT,
	_POST_PROCESS,
	_UI,
	_END
};


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
	CREATE_OBJECT,
	DELETE_OBJECT,
	ADD_CHILD,
	DELETE_RESOURCE,
	LEVEL_CHANGE
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

enum class eANIM_LOOPMODE
{
	NONE,
	NORMAL_LOOP,
	ZIG_ZAG
};

//스타크래프트 맵 데이터 속 "ERA" 안에 들어있는 정보 
enum class eTILESET_INFO : UINT8
{
	BADLANDS = 0x00,
	SPACE_PLATFORM = 0x01,
	INSTALLATION = 0x02,
	ASH_WORLD = 0x03,
	JUNGLE = 0x04,
	DESERT = 0x05,
	ICE = 0x06,
	TWILIGHT = 0x07,
	END = 0x08
};

#include "S_H_Struct.hlsli"
