#include "pch.h"
#include "CUI_Camera.h"

CUI_Camera::CUI_Camera()
	: CUI_Component("##CUI_Camera", eCOMPONENT_COLLIDER2D)
{
	SetName("CameraUI");
}

CUI_Camera::~CUI_Camera()
{
}

int CUI_Camera::render_update()
{
	if (false == CUI_Component::render_update())
		return FALSE;

	return TRUE;
}
