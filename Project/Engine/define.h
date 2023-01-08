#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER 32

#define SINGLETON(type) private: type(); ~type(); friend class CSingleton<type>;
#define TYPE_INDEX(Type) std::type_index(typeid(Type))


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
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링

	// custom
	SCRIPT,			

	END,
};

extern const eCOMPONENT_TYPE g_RenderComponentStart;
extern const eCOMPONENT_TYPE g_RenderComponentEnd;


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

enum class eCB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1

	END,
};


enum eSCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,	

};
#define COLOR_KEY VEC4_3


enum eTEX_PARAM
{
	eTEX_0,
	eTEX_1,
	eTEX_2,
	eTEX_3,
	eTEX_4,
	eTEX_5,
	eTEX_6,
	eTEX_7,

	eTEX_END,
};

enum eSHADER_PIPELINE_STAGE_FLAG : UINT8
{
	eSHADER_PIPELINE_FLAG_VERTEX =		1 << 0,
	eSHADER_PIPELINE_FLAG_HULL =		1 << 1,
	eSHADER_PIPELINE_FLAG_DOMAIN =		1 << 2,
	eSHADER_PIPELINE_FLAG_GEOMETRY =	1 << 3,
	eSHADER_PIPELINE_FLAG_PIXEL =		1 << 4,

	eSHADER_PIPELINE_FLAG_ALL =			UINT8_MAX	
};

//직관적인 방향
enum eDIR_TYPE
{
	eDIR_RIGHT,
	eDIR_UP,
	eDIR_FRONT,
	eDIR_END
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

enum eDEPTHSTENCIL_TYPE
{
	eDEPTHSTENCIL_TYPE_LESS,
	eDEPTHSTENCIL_TYPE_LESS_EQUAL,
	eDEPTHSTENCIL_TYPE_GREATER,
	eDEPTHSTENCIL_TYPE_GREATER_EQUAL,
	eDEPTHSTENCIL_TYPE_NO_WRITE,			//깊이버퍼에 깊이를 기록하지 않음.
	eDEPTHSTENCIL_TYPE_NO_TEST_NO_WRITE,	//깊이 테스트랄 거지치 않고 무조건 픽셀에 색상을 입력. 깊이 버퍼에도 기록하지 않음.
	eDEPTHSTENCIL_TYPE_END
};

enum eBLENDSTATE_TYPE
{
	eBLENDSTATE_DEFAULT,
	eBLENDSTATE_MASK,
	eBLENDSTATE_ALPHABLEND,
	eBLENDSTATE_ONEONE,	//1:1로 섞는 방식 - 컬러키가 검은색인 이미지에 대한 처리
	eBLENDSTATE_END
};

enum eSHADER_DOMAIN
{
	eSHADER_DOMAIN_OPAQUE,
	eSHADER_DOMAIN_MASK,
	eSHADER_DOMAIN_TRANSPARENT,
	eSHADER_DOMAIN_POSTPROCESS,
	eSHADER_DOMAIN_UI,
	eSHADER_DOMAIN_END,
	eSHADER_DOMAIN_UNDEFINED
};

enum eCAMERA_INDEX
{
	eCAMIDX_MAIN,
	eCAMIDX_SUB1,
	eCAMIDX_SUB2,
	eCAMIDX_SUB3,
	eCAMIDX_SUB4,
	eCAMIDX_END
};