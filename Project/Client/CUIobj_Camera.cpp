#include "pch.h"
#include "CUIobj_Camera.h"

CUIobj_Camera::CUIobj_Camera()
	: CUIobj_Component("Camera", eCOMPONENT_TYPE::COLLIDER2D)
{
	SetStrID("CameraUI");
}

CUIobj_Camera::~CUIobj_Camera()
{
}


