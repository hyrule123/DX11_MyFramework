#include "pch.h"
#include "CScriptHolderUI.h"

CScriptHolderUI::CScriptHolderUI()
	: CComponentUI("##CCollider2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Collider2D");
}

CScriptHolderUI::~CScriptHolderUI()
{
}

int CScriptHolderUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;

	return TRUE;
}
