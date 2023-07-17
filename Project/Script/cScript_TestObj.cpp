#include "pch.h"
#include "cScript_TestObj.h"

#include <Engine/cCom_Renderer.h>
#include <Engine/CMaterial.h>
#include <Engine/cCom_Transform.h>

#include <Engine/cTimeMgr.h>
#include <Engine/cKeyMgr.h>

cScript_TestObj::cScript_TestObj(const string_view _strKey)
	: IScript(_strKey)
	, m_ColorKey(1.f, 1.f, 1.f, 1.f)
	, m_MoveSpeed(400.f)
	, m_TurningForceRad(XM_PI / 2.f)
{
}

cScript_TestObj::~cScript_TestObj()
{
}

void cScript_TestObj::BeginCollision(ICollider* _Collider, const Vec3& _v3HitPoint)
{
	GetOwner()->SetLifeSpan(2.f);
}

void cScript_TestObj::init()
{
	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
}

void cScript_TestObj::tick()
{
	Vec3 Scale = Transform().GetRelativeScale();
	Vec3 Rot = Transform().GetRelativeRot();

	if (KEY_PRESSED(eKEY::E))
	{
		Scale += Vec3(1.f, 1.f, 0.f) * DELTA_TIME;
		Rot += Vec3(0.f, 0.f, 1.f) * DELTA_TIME;
	}

	Transform().SetRelativeRot(Rot);
	Transform().SetRelativeScale(Scale);
}
