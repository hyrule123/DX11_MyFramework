#include "pch.h"
#include "CTilemapUI.h"

CTilemapUI::CTilemapUI()
	: CComponentUI("##CCollider2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Collider2D");
}

CTilemapUI::~CTilemapUI()
{
}

int CTilemapUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;

	return TRUE;
}
