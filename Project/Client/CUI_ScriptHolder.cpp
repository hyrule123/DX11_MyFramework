#include "pch.h"
#include "CUI_ScriptHolder.h"

CUI_ScriptHolder::CUI_ScriptHolder()
	: CUI_Component("##CUI_ScriptHolder", eCOMPONENT_COLLIDER2D)
{
	SetName("ScriptHolderUI");
}

CUI_ScriptHolder::~CUI_ScriptHolder()
{
}

int CUI_ScriptHolder::render_update()
{
	if (false == CUI_Component::render_update())
		return FALSE;

	return TRUE;
}
