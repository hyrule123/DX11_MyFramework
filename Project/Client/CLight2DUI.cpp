#include "pch.h"
#include "CLight2DUI.h"

CLight2DUI::CLight2DUI()
	: CComponentUI("##CCollider2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Collider2D");
}

CLight2DUI::~CLight2DUI()
{
}

int CLight2DUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;

	return TRUE;
}
