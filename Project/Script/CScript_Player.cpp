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

#include <Engine/CCollider.h>
#include "defineUser.h"
#include <Engine/define.h>

CScript_Player::CScript_Player()
	: CScript(TYPE_INDEX(CScript_Player))
	, m_ColorKey(0.f, 0.f, 0.f, 0.f)
	, m_MoveSpeed(400.f)
	, m_TurningForceRad(XM_PI / 2.f)
	, m_eState()
{
}

CScript_Player::~CScript_Player()
{
}

void CScript_Player::init()
{
	GetOwner()->Transform()->SetLockRotation(true);
	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY, true);
}

void CScript_Player::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();
	Vector3 vCurRot = Transform()->GetRelativeRot();

	bool RotUpdated = false;
	bool PosUpdated = false;
	//회전 먼저 적용하고
	if (KEY_PRESSED(eKEY::LEFT))
	{
		vCurRot.z += DELTA_TIME * m_TurningForceRad;
		RotUpdated = true;
	}

	if (KEY_PRESSED(eKEY::RIGHT))
	{
		vCurRot.z -= DELTA_TIME * m_TurningForceRad;
		RotUpdated = true;
	}

	//회전한 방향으로 전진
	if (KEY_PRESSED(eKEY::UP))
	{
		//Vec3 Dir = Transform()->GetWorldDir(eDIR_TYPE::RIGHT);
		//vCurPos += DELTA_TIME * m_MoveSpeed * Dir;
		float CurRot = vCurRot.z + 0.5f * XM_PI;

		PosUpdated = true;
		vCurPos.x += DELTA_TIME * m_MoveSpeed * cosf(CurRot);
		vCurPos.y += DELTA_TIME * m_MoveSpeed * sinf(CurRot);
	}

	if (KEY_PRESSED(eKEY::DOWN))
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


	if (KEY_PRESSED(eKEY::Q))
	{
		Vec3 Scale = Transform()->GetRelativeScale();
		Scale += Vec3(DELTA_TIME);
		Transform()->SetRelativeScale(Scale);
	}

			
	if (KEY_TAP(eKEY::_1))
	{
		int a = 0;
		GetOwner()->SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &a);
	}

	else if (KEY_TAP(eKEY::_2))
	{
		int a = 1;
		GetOwner()->SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &a);
	}

	if (KEY_TAP(eKEY::SPACE))
	{
		Shoot();
	}

}

void CScript_Player::OnCollision(CCollider* _pCol)
{
	//커서가 자신과 충돌중임을 확인했을 경우
	if (iLayerCursor == _pCol->GetOwner()->GetLayer())
	{
		//마우스 커서가 한번 클릭됐을 경우
		if (KEY_TAP(eKEY::LBTN))
		{
			m_bSelected = true;
		}
	}


}

void CScript_Player::Shoot()
{
	//CGameObject* Bullet = CResMgr::GetInst()->FindRes<CPrefab>("Bullet")->Instantiate();

	//const Vec3& vPos = GetOwner()->Transform()->GetWorldPos();
	//const Vec3& vDir = GetOwner()->Transform()->GetRelativeDir(eDIR_TYPE::RIGHT);

	//Bullet->ScriptHolder()->GetScript<CScript_Bullet>()->SetDefaultVal(vPos, vDir);

	//tEvent event = { eEVENT_TYPE::CREATE_OBJECT, (DWORD_PTR)Bullet, (DWORD_PTR)1 };

	//CEventMgr::GetInst()->AddEvent(event);
}
