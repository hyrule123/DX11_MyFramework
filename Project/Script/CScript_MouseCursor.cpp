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
	, m_pSelectedGameObject()
{
}

CScript_MouseCursor::~CScript_MouseCursor()
{
}



void CScript_MouseCursor::tick()
{
	if (m_pSelectedGameObject)
	{
		//파괴처리 되었을 경우 선택 해제
		if (m_pSelectedGameObject->GetDestroyed())
			m_pSelectedGameObject = nullptr;
	}

	Vec2 CursorPos = CKeyMgr::GetInst()->GetMousePos();

	//현재 커서의 위치에서 해상도의 반만큼 呼 NDC 좌표와 일치시킨다
	CursorPos -= (g_GlobalVal.vResolution * 0.5f);
	
	const Matrix& matView = CRenderMgr::GetInst()->GetCurCamera()->GetViewMatrix();
	//뷰 매트릭스의 위치 행을 반전시켜 주면 이동행렬을 얻을 수 있음.
	Vec3 Pos = -matView.Translation();

	CursorPos.x += Pos.x;
	CursorPos.y += Pos.y;

	Transform()->SetRelativePos(Vec3(CursorPos, 0.f));
}

void CScript_MouseCursor::OnCollision(CCollider* _Collider)
{
	//주인 게임오브젝트의 주소를 자신의 포인터로 등록
	m_pSelectedGameObject = _Collider->GetOwner();

	CKeyMgr* pKeyMgr = CKeyMgr::GetInst();
	for (int i = 0; i < (int)eKEY_STATE::END; ++i)
	{
		//콜백함수가 등록되어있고 마우스 버튼이 눌렸을 경우
		if (nullptr != m_arrpFuncLBTNCallback[i] && i == (int)(pKeyMgr->GetKeyState(KEY::LBTN)))
			m_arrpFuncLBTNCallback[i](_Collider->GetOwner());

		if (nullptr != m_arrpFuncRBTNCallback[i] && i == (int)(pKeyMgr->GetKeyState(KEY::RBTN)))
			m_arrpFuncRBTNCallback[i](_Collider->GetOwner());
	}
}
