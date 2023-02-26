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
{
}

CScript_MouseCursor::~CScript_MouseCursor()
{
}

void CScript_MouseCursor::tick()
{
	Vec2 CursorPos = CKeyMgr::GetInst()->GetMousePos();

	//현재 커서의 위치에서 해상도의 반만큼 뺴서 NDC 좌표와 일치시킨다
	CursorPos -= (g_GlobalVal.vResolution * 0.5f);
	
	const Matrix& matView = CRenderMgr::GetInst()->GetCurCamera()->GetViewMatrix();
	//뷰 매트릭스의 위치 행을 반전시켜 주면 이동행렬을 얻을 수 있음.
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
