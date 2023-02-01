#include "pch.h"
#include "CUI_Tilemap.h"

CUI_Tilemap::CUI_Tilemap()
	: CUI_Component("##CUI_Tilemap", eCOMPONENT_COLLIDER2D)
{
	SetName("TilemapUI");
}

CUI_Tilemap::~CUI_Tilemap()
{
}

int CUI_Tilemap::render_update()
{
	if (false == CUI_Component::render_update())
		return FALSE;

	return TRUE;
}
