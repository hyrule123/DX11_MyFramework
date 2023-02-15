#include "pch.h"
#include "CCollider2D_Rect.h"

#include "CTransform.h"

CCollider2D_Rect::CCollider2D_Rect()
	: CCollider2D(eCOLLIDER_TYPE_2D::RECT)
	, m_tRectInfo{}
	, m_RectSideInfo{}
{
}

CCollider2D_Rect::CCollider2D_Rect(eCOLLIDER_TYPE_2D _Type)
	: CCollider2D(_Type)
	, m_tRectInfo{}
	, m_RectSideInfo{}
{
}


CCollider2D_Rect::~CCollider2D_Rect()
{
}

void CCollider2D_Rect::UpdateCollider()
{
	CTransform* pTransform = Transform();
	if (nullptr == pTransform)
		return;

	//회전이 적용되지 않는 충돌체이므로 크기와 사이즈만 받아 온다.
	

	
}

void CCollider2D_Rect::DebugRender()
{
}
