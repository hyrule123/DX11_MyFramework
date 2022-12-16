#include "pch.h"
#include "CCameraMoveScript.h"


CCameraMoveScript::CCameraMoveScript() :
	m_CamSpeed(400.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	CTransform* pTransform = Transform();
	Vec3 CamPos = pTransform->GetRelativePos();
	Vec3 CamRot = pTransform->GetRelativeRot();

	if (KEY_PRESSED(KEY::W))
	{
		CamPos.y += DT * m_CamSpeed;
	}
	if (KEY_PRESSED(KEY::S))
	{
		CamPos.y -= DT * m_CamSpeed;
	}
	if (KEY_PRESSED(KEY::A))
	{
		CamPos.x -= DT * m_CamSpeed;
	}
	if (KEY_PRESSED(KEY::D))
	{
		CamPos.x += DT * m_CamSpeed;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		CamRot.x += DT * XM_PI;
		CamRot.y += DT * XM_PI;
	}

	pTransform->SetRelativeRot(CamRot);

	pTransform->SetRelativePos(CamPos);
}

