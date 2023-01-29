#include "pch.h"
#include "CAnimator2DUI.h"

CAnimator2DUI::CAnimator2DUI()
	: CComponentUI("##CAnimator2DUI", eCOMPONENT_COLLIDER2D)
{
	SetName("Animator2DUI");
}

CAnimator2DUI::~CAnimator2DUI()
{
}

int CAnimator2DUI::render_update()
{
	if (false == CComponentUI::render_update())
		return FALSE;

	return TRUE;
}