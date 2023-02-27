#include "pch.h"
#include "CScript_MouseCursor.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CTransform.h>
#include <Engine/CCollider.h>

#include <Engine/CGameObject.h>


CScript_MouseCursor::CScript_MouseCursor()
	: CScript(TYPE_INDEX(CScript_MouseCursor))
	, m_arrpFuncLBTNCallback{}
	, m_arrpFuncRBTNCallback{}
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

	CKeyMgr* pKeyMgr = CKeyMgr::GetInst();
	for (int i = 0; i < (int)eKEY_STATE::END; ++i)
	{
		if (nullptr != m_arrpFuncLBTNCallback[i] && i == (int)(pKeyMgr->GetKeyState(KEY::LBTN)))
			m_arrpFuncLBTNCallback[i](_Collider->GetOwner());

		if (nullptr != m_arrpFuncRBTNCallback[i] && i == (int)(pKeyMgr->GetKeyState(KEY::RBTN)))
			m_arrpFuncRBTNCallback[i](_Collider->GetOwner());
	}
}
