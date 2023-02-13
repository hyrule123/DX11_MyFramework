#include "pch.h"
#include "CPlayerScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

//Prefab instantiate
#include "CResMgr.h"
#include "CEventMgr.h"
#include "CBulletScript.h"
#include "CScriptHolder.h"


CPlayerScript::CPlayerScript()
	: CScript(TYPE_INDEX(CPlayerScript))
	, m_ColorKey(1.f, 1.f, 1.f, 1.f)
	, m_MoveSpeed(400.f)
	, m_TurningForceRad(XM_PI / 2.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::init()
{
	//������ۿ� �÷�Ű�� ����, �ȼ� ���̴��� ������۰� ���޵ǵ��� ����
	RenderCom()->GetMaterial()->SetScalarParam(COLOR_KEY, &m_ColorKey);
}

void CPlayerScript::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();
	Vector3 vCurRot = Transform()->GetRelativeRot();

	//ȸ�� ���� �����ϰ�
	if (KEY_PRESSED(KEY::LEFT))
	{
		vCurRot.z += DELTA_TIME * m_TurningForceRad;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vCurRot.z -= DELTA_TIME * m_TurningForceRad;
	}

	//ȸ���� �������� ����
	if (KEY_PRESSED(KEY::UP))
	{
		Vec3 Dir = Transform()->GetRelativeDir(eDIR_TYPE::RIGHT);
		vCurPos += DELTA_TIME * m_MoveSpeed * Dir;

		//vCurPos.x += DELTA_TIME * m_MoveSpeed * cosf(vCurRot.z);
		//vCurPos.y += DELTA_TIME * m_MoveSpeed * sinf(vCurRot.z);
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		Vec3 Dir = Transform()->GetRelativeDir(eDIR_TYPE::RIGHT);
		vCurPos -= DELTA_TIME * m_MoveSpeed * Dir;

		//vCurPos.x -= DELTA_TIME * m_MoveSpeed * cosf(vCurRot.z);
		//vCurPos.y -= DELTA_TIME * m_MoveSpeed * sinf(vCurRot.z);
	}

	Transform()->SetRelativePos(vCurPos);
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
		MeshRender()->GetMaterial()->SetScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &a);
	}

	else if (KEY_TAP(KEY::_2))
	{
		int a = 1;
		MeshRender()->GetMaterial()->SetScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &a);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		Shoot();
	}

}

void CPlayerScript::Shoot()
{
	CGameObject* Bullet = CResMgr::GetInst()->FindRes<CPrefab>("Bullet")->Instantiate();

	const Vec3& vPos = GetOwner()->Transform()->GetWorldPos();
	const Vec3& vDir = GetOwner()->Transform()->GetRelativeDir(eDIR_TYPE::RIGHT);

	Bullet->ScriptHolder()->GetScript<CBulletScript>()->SetDefaultVal(vPos, vDir);

	tEvent event = { eEVENT_TYPE::CREATE_OBJECT, (DWORD_PTR)Bullet, (DWORD_PTR)1 };

	CEventMgr::GetInst()->AddEvent(event);
}
