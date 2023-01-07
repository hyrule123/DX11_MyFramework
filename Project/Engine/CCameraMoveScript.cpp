#include "pch.h"
#include "CCameraMoveScript.h"

#include "CTransform.h"
#include "CCamera.h"

//마우스 좌표 받아오는 용도
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


	if (KEY_PRESSED(KEY::W))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_UP);

		CamPos += DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::S))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_UP);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::A))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_RIGHT);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::D))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_RIGHT);

		CamPos += DT * m_CamSpeed * Dir;
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
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_FRONT);

		CamPos += DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::S))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_FRONT);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::A))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_RIGHT);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(KEY::D))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::eDIR_RIGHT);

		CamPos += DT * m_CamSpeed * Dir;
	}

	//우클릭시 시점 전환
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 MouseDir = CKeyMgr::GetInst()->GetMouseDir();

		//y축을 돌려야 좌우로 회전한다.
		CamRot.y += DT * MouseDir.x * m_TurningForceRad;

		//x축을 돌려야 상하로 회전한다.
		CamRot.x -= DT * MouseDir.y * m_TurningForceRad;
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

