#include "pch.h"
#include "cScript_MainCamSC_InGame.h"

#include <Engine/cGameObject.h>

#include <Engine/cKeyMgr.h>
#include <Engine/cTimeMgr.h>

#include <Engine/cCom_Camera.h>

constexpr float fCamSpeed = 500.f;

cScript_MainCamSC_InGame::cScript_MainCamSC_InGame()
	: IScript()
	, m_fCamSpeed(fCamSpeed)
{
}

cScript_MainCamSC_InGame::~cScript_MainCamSC_InGame()
{
}

void cScript_MainCamSC_InGame::Init()
{
	m_pCam = GetOwner()->Camera();
	assert(m_pCam);
}

void cScript_MainCamSC_InGame::Tick()
{
	CameraMove();
}

void cScript_MainCamSC_InGame::CameraMove()
{
	cTransform& pTransform = GetOwner()->Transform();
	
	Vec3 MovedPos;
	float DT = DELTA_TIME;
	bool PosChanged = false;

	if (KEY_PRESSED(eKEY::UP))
	{
		MovedPos += DT * m_fCamSpeed * Vec3::Up;
		PosChanged = true;
	}
	if (KEY_PRESSED(eKEY::DOWN))
	{
		MovedPos += DT * m_fCamSpeed * Vec3::Down;
		PosChanged = true;
	}
	if (KEY_PRESSED(eKEY::LEFT))
	{
		MovedPos += DT * m_fCamSpeed * Vec3::Left;
		PosChanged = true;
	}
	if (KEY_PRESSED(eKEY::RIGHT))
	{
		MovedPos += DT * m_fCamSpeed * Vec3::Right;
		PosChanged = true;
	}

	if (PosChanged)
	{
		pTransform.AddRelativePos(MovedPos);
	}
}
