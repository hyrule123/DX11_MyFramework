#include "pch.h"
#include "CCameraUI.h"

CCameraUI::CCameraUI()
	: CComponentUI("##CCollider2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Collider2D");
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
