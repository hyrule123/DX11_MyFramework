#include "pch.h"
#include "CUI_Light2D.h"

CUI_Light2D::CUI_Light2D()
	: CUI_Component("##CUI_Light2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Light2DUI");
}

CUI_Light2D::~CUI_Light2D()
{
}

int CUI_Light2D::render_update()
{
	if (false == CUI_Component::render_update())
		return FALSE;

	return TRUE;
}
