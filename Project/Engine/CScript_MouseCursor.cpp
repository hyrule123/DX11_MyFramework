#include "pch.h"
#include "CScript_MouseCursor.h"

#include "CKeyMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CCollider.h"

#include "CGameObject.h"


CScript_MouseCursor::CScript_MouseCursor()
	: CScript(TYPE_INDEX(CScript_MouseCursor))
{
}

CScript_MouseCursor::~CScript_MouseCursor()
{
}

void CScript_MouseCursor::tick()
{
	Vec2 CursorPos = CKeyMgr::GetInst()->GetMousePos();

	//���� Ŀ���� ��ġ���� �ػ��� �ݸ�ŭ ���� NDC ��ǥ�� ��ġ��Ų��
	CursorPos -= (g_GlobalVal.vResolution * 0.5f);
	
	const Matrix& matView = CRenderMgr::GetInst()->GetCurCamera()->GetViewMatrix();
	//�� ��Ʈ������ ��ġ ���� �������� �ָ� �̵������ ���� �� ����.
	Vec3 Pos = matView.Translation();

	CursorPos.x += Pos.x;
	CursorPos.y += Pos.y;

	Transform()->SetRelativePos(Vec3(CursorPos, 0.f));
}

void CScript_MouseCursor::OnCollision(CCollider* _Collider)
{
	if (KEY_TAP(KEY::LBTN))
	{
		::DestroyObject(_Collider->GetOwner());
	}
}
