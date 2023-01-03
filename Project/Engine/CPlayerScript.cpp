#include "pch.h"
#include "CPlayerScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"

CPlayerScript::CPlayerScript() : 
	m_ColorKey(1.f, 1.f, 1.f, 1.f),
	m_MoveSpeed(400.f),
	m_TurningForceRad(XM_PI / 2.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::init()
{
	//상수버퍼에 컬러키를 전달, 픽셀 쉐이더에 상수버퍼가 전달되도록 설정
	MeshRender()->GetMaterial()->SetScalarParam(COLOR_KEY, &m_ColorKey);
}

void CPlayerScript::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();
	Vector3 vCurRot = Transform()->GetRelativeRot();

	//회전 먼저 적용하고
	if (KEY_PRESSED(KEY::LEFT))
	{
		vCurRot.z += DT * m_TurningForceRad;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vCurRot.z -= DT * m_TurningForceRad;
	}

	//회전한 방향으로 전진
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
