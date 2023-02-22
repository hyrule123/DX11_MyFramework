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

//���ο� ������Ʈ�� ������� ���
//eCOMPONENT_TYPE�� ������Ʈ �߰�
//components.h�� ������Ʈ �վ��ֱ�
enum class eCOMPONENT_TYPE
{
	// update
	TRANSFORM,		// ��ġ, ũ��, ȸ��
	COLLIDER2D,		// 2���� �浹
	COLLIDER3D,		// 3���� �浹
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,			// 2���� ����
	LIGHT3D,			// 3���� ����
	CAMERA,			// Camera

	// render
	MESH_RENDER,		// �⺻���� ������
	PARTICLE_SYSTEM, // ���� ������
	TILEMAP,		// 2���� Ÿ��
	LANDSCAPE,		// 3���� ����
	DECAL,			// ���� ������

	// custom
	SCRIPT_HOLDER,			

	END
};
extern const eCOMPONENT_TYPE g_RenderComIdxStart;
extern const eCOMPONENT_TYPE g_RenderComIdxEnd;


enum class eRES_TYPE
{
	MESH,			// ����
	MESHDATA,
	MATERIAL,
	
	TEXTURE,		// �̹���
	ANIM2D_SPRITE,
	SOUND,

	PREFAB,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

constexpr const char* g_eRES_TYPE_strName[(int)eCOMPONENT_TYPE::END]
{
	"MESHDATA",
	"MATERIAL",
	"PREFAB",
	"MESH",
	"TEXTURE",
	"SOUND",
	"GRAPHICS_SHADER",
	"COMPUTE_SHADER",
	"END"
};

constexpr const wchar_t* g_eRES_TYPE_lstrName[(int)eCOMPONENT_TYPE::END]
{
	L"MESHDATA",
	L"MATERIAL",
	L"PREFAB",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"GRAPHICS_SHADER",
	L"COMPUTE_SHADER",
	L"END"
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



//�������� ����
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
	NO_WRITE,			//���̹��ۿ� ���̸� ������� ����.
	NO_TEST_NO_WRITE,	//���� �׽�Ʈ�� ����ġ �ʰ� ������ �ȼ��� ������ �Է�. ���� ���ۿ��� ������� ����.
	END
};

enum class eBLENDSTATE_TYPE
{
	DEFAULT,
	MASK,
	ALPHA_BLEND,
	ONE_ONE,	//1:1�� ���� ��� - �÷�Ű�� �������� �̹����� ���� ó��
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

//���� ���ε��Ǿ� �ִ� ��. �÷��׷ε� ��� ����
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