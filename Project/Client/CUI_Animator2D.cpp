#include "pch.h"
#include "CUI_Animator2D.h"

CUI_Animator2D::CUI_Animator2D()
	: CUI_Component("##CUI_Animator2D", eCOMPONENT_COLLIDER2D)
{
	SetName("Animator2DUI");
}

CUI_Animator2D::~CUI_Animator2D()
{
}

int CUI_Animator2D::render_update()
{
	if (false == CUI_Component::render_update())
		return FALSE;



	return TRUE;
}