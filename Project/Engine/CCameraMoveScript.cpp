#include "pch.h"
#include "CCameraMoveScript.h"

#include "CTransform.h"
#include "CCamera.h"

//���콺 ��ǥ �޾ƿ��� �뵵
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CCameraMoveScript::CCameraMoveScript() 
	: CScript(TYPE_INDEX(CCameraMoveScript))
	, m_CamSpeed(400.f)
	, m_TurningForceRad(XM_PI)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Camera2DMove()
{
	CTransform* pTransform = Transform();
	Vec3 CamPos = pTransform->GetRelativePos();
	Vec3 CamRot = pTransform->GetRelativeRot();

	float DT = DELTA_TIME;

	if (KEY_PRESSED(KEY::W))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::UP);

		CamPos += DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::S))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::UP);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::A))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::D))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos += DT * m_CamSpeed * Dir;
	}

	if (KEY_PRESSED(KEY::C))
	{
		Camera()->Zoom2D(1.0f + 0.5f * DT);
	}
	else if (KEY_PRESSED(KEY::X))
	{
		Camera()->Zoom2D(1.0f - 0.5f * DT);
	}


	if (KEY_PRESSED(KEY::Z))
	{
		CamRot.x += DT * XM_PI;
		CamRot.y += DT * XM_PI;
	}

	pTransform->SetRelativeRot(CamRot);
	pTransform->SetRelativePos(CamPos);
}

void CCameraMoveScript::Camera3DMove()
{
	CTransform* pTransform = Transform();
	Vec3 CamPos = pTransform->GetRelativePos();
	Vec3 CamRot = pTransform->GetRelativeRot();


	if (KEY_PRESSED(KEY::W))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::FRONT);

		CamPos += DELTA_TIME * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::S))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::FRONT);

		CamPos -= DELTA_TIME * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::A))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos -= DELTA_TIME * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::D))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos += DELTA_TIME * m_CamSpeed * Dir;
	}

	//��Ŭ���� ���� ��ȯ
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 MouseDir = CKeyMgr::GetInst()->GetMouseDir();

		//y���� ������ �¿�� ȸ���Ѵ�.
		CamRot.y += DELTA_TIME * MouseDir.x * m_TurningForceRad;

		//x���� ������ ���Ϸ� ȸ���Ѵ�.
		CamRot.x -= DELTA_TIME * MouseDir.y * m_TurningForceRad;
	}

	pTransform->SetRelativePos(CamPos);
	pTransform->SetRelativeRot(CamRot);
}

void CCameraMoveScript::tick()
{
	ePROJ_TYPE ProjType = Camera()->GetProjType();
	bool VTapped = false;
	if (KEY_TAP(KEY::V))
		VTapped = true;

	switch (ProjType)
	{
	case ePROJ_TYPE::ORTHOGRAPHY:
	{
		if (VTapped)
		{
			Camera()->SetProjType(ePROJ_TYPE::PERSPECTIVE);
			break;
		}

		Camera2DMove();
		break;
	}
	case ePROJ_TYPE::PERSPECTIVE:
	{
		if (VTapped)
		{
			Camera()->SetProjType(ePROJ_TYPE::ORTHOGRAPHY);
			break;
		}

		Camera3DMove();
		break;
	}
	}


}

