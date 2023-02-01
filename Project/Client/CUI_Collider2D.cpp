#include "pch.h"
#include "CUI_Collider2D.h"

CUI_Collider2D::CUI_Collider2D()
	: CUI_Component("##CCollider2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Collider2D");
}

CUI_Collider2D::~CUI_Collider2D()
{
}

int CUI_Collider2D::render_update()
{
	if (false == CUI_Component::render_update())
		return FALSE;

	return TRUE;
}
