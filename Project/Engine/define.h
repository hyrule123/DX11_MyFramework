#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DELTA_TIME CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER 32

#define SINGLETON(type) private: type(); ~type(); friend class CSingleton<type>;
#define TYPE_INDEX(Type) std::type_index(typeid(Type))

#define FLT_MAX_NEG -FLT_MAX

#define BITMASK(n) (1 << n)

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

	END
};
extern const eCOMPONENT_TYPE g_RenderComIdxStart;
extern const eCOMPONENT_TYPE g_RenderComIdxEnd;


enum class eRES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};



namespace eSHADER_PIPELINE_STAGE
{
	enum FLAG : UINT8
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

enum class eRASTERIZER_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END
};

enum class eDEPTHSTENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			//깊이버퍼에 깊이를 기록하지 않음.
	NO_TEST_NO_WRITE,	//깊이 테스트랄 거지치 않고 무조건 픽셀에 색상을 입력. 깊이 버퍼에도 기록하지 않음.
	END
};

enum class eBLENDSTATE_TYPE
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

enum class eCAMERA_INDEX : UINT
{
	MAIN,
	SUB1,
	SUB2,
	SUB3,
	SUB4,
	END
};

enum class eEVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	ADD_CHILD,
	DELETE_RESOURCE,
	LEVEL_CHANGE
};

enum class eSHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	END,
};

enum class eCOLLIDER_TYPE
{
	_2D_RECT,
	_2D_CIRCLE,
	_2D_OBB
};

enum eDIMENSION_TYPE : UINT
{
	eDIMENSION_2D = 2u,
	eDIMENSION_3D = 3u
};

enum class eSTRUCT_BUFFER_TYPE
{
	READ_ONLY,  //SRV ONLY
	READ_WRITE  //SRV + UAV(Compute Shader)
} typedef eSTRUCT_BUFFER_BIND_TYPE;

//현재 바인딩되어 있는 뷰. 플래그로도 사용 가능
namespace eCURRENT_BOUND_VIEW
{
	enum eCURRENT_BOUND_VIEW
	{
		NONE = 0,
		SRV = 1 << 0,
		UAV = 1 << 1,
		RTV = 1 << 2,
		DSV = 1 << 3
	};
}
