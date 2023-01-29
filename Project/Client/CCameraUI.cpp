#include "pch.h"
#include "CCameraUI.h"

CCameraUI::CCameraUI()
	: CComponentUI("##CCameraUI", eCOMPONENT_COLLIDER2D)
{
	SetName("CameraUI");
}

CCameraUI::~CCameraUI()
{
}

int CCameraUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;

	return TRUE;
}
