#include "pch.h"
#include "CUI_Camera.h"

CUI_Camera::CUI_Camera()
	: CUI_Component("Camera", eCOMPONENT_TYPE::COLLIDER2D)
{
	SetStrID("CameraUI");
}

CUI_Camera::~CUI_Camera()
{
}


