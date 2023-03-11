#include "pch.h"
#include "CScript_Player.h"

#include "CScript_Bullet.h"

#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>
#include <Engine/CTransform.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CKeyMgr.h>

//Prefab instantiate
#include <Engine/CResMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CScriptHolder.h>

CScript_Player::CScript_Player()
	: CScript(TYPE_INDEX(CScript_Player))
	, m_ColorKey(1.f, 1.f, 1.f, 1.f)
	, m_MoveSpeed(400.f)
	, m_TurningForceRad(XM_PI / 2.f)
{
}

CScript_Player::~CScript_Player()
{
}

void CScript_Player::init()
{
	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	//GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
}

void CScript_Player::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();
	Vector3 vCurRot = Transform()->GetRelativeRot();

	bool RotUpdated = false;
	bool PosUpdated = false;
	//회전 먼저 적용하고
	if (KEY_PRESSED(KEY::LEFT))
	{
		vCurRot.z += DELTA_TIME * m_TurningForceRad;
		RotUpdated = true;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vCurRot.z -= DELTA_TIME * m_TurningForceRad;
		RotUpdated = true;
	}

	//회전한 방향으로 전진
	if (KEY_PRESSED(KEY::UP))
	{
		//Vec3 Dir = Transform()->GetWorldDir(eDIR_TYPE::RIGHT);
		//vCurPos += DELTA_TIME * m_MoveSpeed * Dir;
		float CurRot = vCurRot.z + 0.5f * XM_PI;

		PosUpdated = true;
		vCurPos.x += DELTA_TIME * m_MoveSpeed * cosf(CurRot);
		vCurPos.y += DELTA_TIME * m_MoveSpeed * sinf(CurRot);
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		//Vec3 Dir = Transform()->GetWorldDir(eDIR_TYPE::RIGHT);
		//vCurPos -= DELTA_TIME * m_MoveSpeed * Dir;
		float CurRot = vCurRot.z + 0.5f * XM_PI;

		PosUpdated = true;
		vCurPos.x -= DELTA_TIME * m_MoveSpeed * cosf(CurRot);
		vCurPos.y -= DELTA_TIME * m_MoveSpeed * sinf(CurRot);
	}

	if(true == PosUpdated)
		Transform()->SetRelativePos(vCurPos);
	if(true == RotUpdated)
		Transform()->SetRelativeRot(vCurRot);


	if (KEY_PRESSED(KEY::Q))
	{
		Vec3 Scale = Transform()->GetRelativeScale();
		Scale += Vec3(DELTA_TIME);
		Transform()->SetRelativeScale(Scale);
	}

			
	if (KEY_TAP(KEY::_1))
	{
		int a = 0;
		GetOwner()->SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &a);
	}

	else if (KEY_TAP(KEY::_2))
	{
		int a = 1;
		GetOwner()->SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &a);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		Shoot();
	}

}

void CScript_Player::Shoot()
{
	CGameObject* Bullet = CResMgr::GetInst()->FindRes<CPrefab>("Bullet")->Instantiate();

	const Vec3& vPos = GetOwner()->Transform()->GetWorldPos();
	const Vec3& vDir = GetOwner()->Transform()->GetRelativeDir(eDIR_TYPE::RIGHT);

	Bullet->ScriptHolder()->GetScript<CScript_Bullet>()->SetDefaultVal(vPos, vDir);

	tEvent event = { eEVENT_TYPE::CREATE_OBJECT, (DWORD_PTR)Bullet, (DWORD_PTR)1 };

	CEventMgr::GetInst()->AddEvent(event);
}
