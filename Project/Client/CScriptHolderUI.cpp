#include "pch.h"
#include "CScriptHolderUI.h"

CScriptHolderUI::CScriptHolderUI()
	: CComponentUI("##CScriptHolderUI", eCOMPONENT_COLLIDER2D)
{
	SetName("ScriptHolderUI");
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
