#include "pch.h"
#include "CTestObjScript.h"

#include "pch.h"
#include "CTestObjScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

CTestObjScript::CTestObjScript()
	: CScript(TYPE_INDEX(CTestObjScript))
	, m_ColorKey(1.f, 1.f, 1.f, 1.f)
	, m_MoveSpeed(400.f)
	, m_TurningForceRad(XM_PI / 2.f)
{
}

CTestObjScript::~CTestObjScript()
{
}

void CTestObjScript::BeginCollision(CCollider* _pCol)
{
	GetOwner()->SetLifeSpan(2.f);
}

void CTestObjScript::init()
{
	//������ۿ� �÷�Ű�� ����, �ȼ� ���̴��� ������۰� ���޵ǵ��� ����
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_STD2D_COLORKEY, &m_ColorKey);
}

void CTestObjScript::tick()
{
	
	Vec3 Scale = Transform()->GetRelativeScale();
	Vec3 Rot = Transform()->GetRelativeRot();

	if (KEY_PRESSED(KEY::E))
	{
		Scale += Vec3(1.f, 1.f, 0.f) * DELTA_TIME;
		Rot += Vec3(0.f, 0.f, 1.f) * DELTA_TIME;
	}

	Transform()->SetRelativeRot(Rot);
	Transform()->SetRelativeScale(Scale);

}
