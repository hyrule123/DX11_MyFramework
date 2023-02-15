#include "pch.h"
#include "CUI_Collider2D.h"

CUI_Collider2D::CUI_Collider2D()
	: CUI_Component("CCollider2D", eCOMPONENT_TYPE::COLLIDER2D)
{
	SetStrID("Collider2D");
}

CUI_Collider2D::~CUI_Collider2D()
{
}

