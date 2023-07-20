#include "pch.h"
#include "cScript_TestObj.h"

#include <Engine/cGameObject.h>
#include <Engine/cTransform.h>
#include <Engine/components.h>

#include <Engine/IRenderer.h>
#include <Engine/cMaterial.h>
#include <Engine/cTransform.h>

#include <Engine/cTimeMgr.h>
#include <Engine/cKeyMgr.h>

cScript_TestObj::cScript_TestObj()
	: m_ColorKey(1.f, 1.f, 1.f, 1.f)
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

void cScript_TestObj::Init()
{
	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
}

void cScript_TestObj::Tick()
{
	Vec3 Scale = GetOwner()->Transform().GetRelativeScale();
	Vec3 Rot = GetOwner()->Transform().GetRelativeRot();

	if (KEY_PRESSED(eKEY::E))
	{
		Scale += Vec3(1.f, 1.f, 0.f) * DELTA_TIME;
		Rot += Vec3(0.f, 0.f, 1.f) * DELTA_TIME;
	}

	GetOwner()->Transform().SetRelativeRot(Rot);
	GetOwner()->Transform().SetRelativeScale(Scale);
}
