#include "pch.h"
#include "CLight2DUI.h"

CLight2DUI::CLight2DUI()
	: CComponentUI("##CLight2DUI", eCOMPONENT_COLLIDER2D)
{
	SetName("Light2DUI");
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
