#include "pch.h"
#include "CUIobj_Animator2D.h"

CUIobj_Animator2D::CUIobj_Animator2D()
	: CUIobj_Component("Animator2D", eCOMPONENT_TYPE::COLLIDER2D)
{
	SetStrID("Animator2DUI");
}

CUIobj_Animator2D::~CUIobj_Animator2D()
{
}
