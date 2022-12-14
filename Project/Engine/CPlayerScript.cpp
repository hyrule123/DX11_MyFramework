#include "pch.h"
#include "CPlayerScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"


CPlayerScript::CPlayerScript() : 
	m_ColorKey(1.f, 1.f, 1.f, 1.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::UP))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.y += DT * 1.f;
		}
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.y -= DT * 1.f;
		}
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x -= DT * 1.f;
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x += DT * 1.f;
		}
	}

	Transform()->SetRelativePos(vCurPos);

	Vector3 vCurRot = Transform()->GetRelativeRot();
	if (KEY_PRESSED(KEY::A))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurRot.z += DT * XM_PI;
		}
	}
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

	//������ۿ� �÷�Ű�� ����
	MeshRender()->GetMaterial()->SetScalarParam(COLOR_KEY, &m_ColorKey);
}
