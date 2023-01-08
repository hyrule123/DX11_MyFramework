#include "pch.h"
#include "CPlayerScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

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
	MeshRender()->GetMaterial()->SetScalarParam(COLOR_KEY, &m_ColorKey);
}

void CPlayerScript::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();
	Vector3 vCurRot = Transform()->GetRelativeRot();

	//ȸ�� ���� �����ϰ�
	if (KEY_PRESSED(KEY::LEFT))
	{
		vCurRot.z += DT * m_TurningForceRad;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vCurRot.z -= DT * m_TurningForceRad;
	}

	//ȸ���� �������� ����
	if (KEY_PRESSED(KEY::UP))
	{
		Vec3 Dir = Transform()->GetRelativeDir(eDIR_TYPE::eDIR_RIGHT);
		vCurPos += DT * m_MoveSpeed * Dir;

		//vCurPos.x += DT * m_MoveSpeed * cosf(vCurRot.z);
		//vCurPos.y += DT * m_MoveSpeed * sinf(vCurRot.z);
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		Vec3 Dir = Transform()->GetRelativeDir(eDIR_TYPE::eDIR_RIGHT);
		vCurPos -= DT * m_MoveSpeed * Dir;

		//vCurPos.x -= DT * m_MoveSpeed * cosf(vCurRot.z);
		//vCurPos.y -= DT * m_MoveSpeed * sinf(vCurRot.z);
	}

	Transform()->SetRelativePos(vCurPos);
	Transform()->SetRelativeRot(vCurRot);


	if (KEY_PRESSED(KEY::Q))
	{
		Vec3 Scale = Transform()->GetRelativeScale();
		Scale += Vec3(DT);
		Transform()->SetRelativeScale(Scale);
	}

			
	if (KEY_TAP(KEY::_1))
	{
		int a = 0;
		MeshRender()->GetMaterial()->SetScalarParam(INT_0, &a);
	}

	else if (KEY_TAP(KEY::_2))
	{
		int a = 1;
		MeshRender()->GetMaterial()->SetScalarParam(INT_0, &a);
	}


}
