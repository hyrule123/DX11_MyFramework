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

	//ȸ���� ������� �ʴ� �浹ü�̹Ƿ� ũ��� ����� �޾� �´�.
	

	
}

void CCollider2D_Rect::DebugRender()
{
}
