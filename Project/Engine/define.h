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
	LIGHT2D,		// 2���� ����
	LIGHT3D,		// 3���� ����
	CAMERA,			// Camera

	// render
	MESHRENDER,		// �⺻���� ������
	PARTICLESYSTEM, // ���� ������
	TILEMAP,		// 2���� Ÿ��
	LANDSCAPE,		// 3���� ����
	DECAL,			// ���� ������

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

	MESH,			// ����
	TEXTURE,		// �̹���
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
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_END,
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

//�������� ����
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
	eDEPTHSTENCIL_TYPE_NO_WRITE,			//���̹��ۿ� ���̸� ������� ����.
	eDEPTHSTENCIL_TYPE_NO_TEST_NO_WRITE,	//���� �׽�Ʈ�� ����ġ �ʰ� ������ �ȼ��� ������ �Է�. ���� ���ۿ��� ������� ����.
	eDEPTHSTENCIL_TYPE_END
};

enum eBLENDSTATE_TYPE
{
	eBLENDSTATE_DEFAULT,
	eBLENDSTATE_MASK,
	eBLENDSTATE_ALPHABLEND,
	eBLENDSTATE_ONEONE,	//1:1�� ���� ��� - �÷�Ű�� �������� �̹����� ���� ó��
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