#include "pch.h"
#include "CScript_CameraMove.h"

#include <Engine/CTransform.h>
#include <Engine/CCamera.h>

//마우스 좌표 받아오는 용도
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

CScript_CameraMove::CScript_CameraMove(const string& _strKey)
	: CScript(_strKey)
	, m_CamSpeed(400.f)
	, m_TurningForceRad(XM_PI)
{
}

CScript_CameraMove::~CScript_CameraMove()
{
}

void CScript_CameraMove::Camera2DMove()
{
	CTransform* pTransform = Transform();
	Vec3 CamPos = pTransform->GetRelativePos();
	Vec3 CamRot = pTransform->GetRelativeRot();

	float DT = DELTA_TIME;

	if (KEY_PRESSED(eKEY::W))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::UP);

		CamPos += DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(eKEY::S))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::UP);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(eKEY::A))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos -= DT * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(eKEY::D))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos += DT * m_CamSpeed * Dir;
	}

	if (KEY_PRESSED(eKEY::C))
	{
		Camera()->Zoom2D(1.0f + 0.5f * DT);
	}
	else if (KEY_PRESSED(eKEY::X))
	{
		Camera()->Zoom2D(1.0f - 0.5f * DT);
	}


	if (KEY_PRESSED(eKEY::Z))
	{
		CamRot.x += DT * XM_PI;
		CamRot.y += DT * XM_PI;
	}

	pTransform->SetRelativeRot(CamRot);
	pTransform->SetRelativePos(CamPos);
}

void CScript_CameraMove::Camera3DMove()
{
	CTransform* pTransform = Transform();
	Vec3 CamPos = pTransform->GetRelativePos();
	Vec3 CamRot = pTransform->GetRelativeRot();


	if (KEY_PRESSED(eKEY::W))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::FRONT);

		CamPos += DELTA_TIME * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(eKEY::S))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::FRONT);

		CamPos -= DELTA_TIME * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(eKEY::A))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos -= DELTA_TIME * m_CamSpeed * Dir;
	}
	if (KEY_PRESSED(eKEY::D))
	{
		Vec3 Dir = pTransform->GetRelativeDir(eDIR_TYPE::RIGHT);

		CamPos += DELTA_TIME * m_CamSpeed * Dir;
	}

	//우클릭시 시점 전환
	if (KEY_PRESSED(eKEY::RBTN))
	{
		Vec2 MouseDir = CKeyMgr::GetInst()->GetMouseDir();

		//y축을 돌려야 좌우로 회전한다.
		CamRot.y += DELTA_TIME * MouseDir.x * m_TurningForceRad;

		//x축을 돌려야 상하로 회전한다.
		CamRot.x -= DELTA_TIME * MouseDir.y * m_TurningForceRad;
	}

	pTransform->SetRelativePos(CamPos);
	pTransform->SetRelativeRot(CamRot);
}

void CScript_CameraMove::tick()
{
	ePROJ_TYPE ProjType = Camera()->GetProjType();
	bool VTapped = false;
	if (KEY_DOWN(eKEY::V))
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

