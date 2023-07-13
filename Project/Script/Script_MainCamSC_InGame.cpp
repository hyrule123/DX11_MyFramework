#include "pch.h"
#include "CScript_MainCamSC_InGame.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>

#include <Engine/CCamera.h>

constexpr float fCamSpeed = 500.f;

CScript_MainCamSC_InGame::CScript_MainCamSC_InGame(const string_view _strKey)
	: CScript(_strKey)
	, m_fCamSpeed(fCamSpeed)
{
}

CScript_MainCamSC_InGame::~CScript_MainCamSC_InGame()
{
}

void CScript_MainCamSC_InGame::init()
{
	m_pCam = Camera();
	assert(m_pCam);
}

void CScript_MainCamSC_InGame::tick()
{
	CameraMove();
}

void CScript_MainCamSC_InGame::CameraMove()
{
	CTransform& pTransform = Transform();
	
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
