#include "pch.h"
#include "CTilemapUI.h"

CTilemapUI::CTilemapUI()
	: CComponentUI("##CTilemapUI", eCOMPONENT_COLLIDER2D)
{
	SetName("TilemapUI");
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
