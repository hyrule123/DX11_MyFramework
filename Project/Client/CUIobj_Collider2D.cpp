#include "pch.h"
#include "CUIobj_Collider2D.h"

CUIobj_Collider2D::CUIobj_Collider2D()
	: CUIobj_Component("CCollider2D", eCOMPONENT_TYPE::COLLIDER2D)
{
	SetStrID("Collider2D");
}

CUIobj_Collider2D::~CUIobj_Collider2D()
{

}

