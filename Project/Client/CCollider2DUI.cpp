#include "pch.h"
#include "CCollider2DUI.h"

CCollider2DUI::CCollider2DUI()
	: CComponentUI("##CCollider2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Collider2D");
}

CCollider2DUI::~CCollider2DUI()
{
}

int CCollider2DUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;

	return TRUE;
}
