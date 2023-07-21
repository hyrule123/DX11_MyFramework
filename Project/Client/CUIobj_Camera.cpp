#include "pch.h"
#include "cUIobj_Camera.h"

cUIobj_Camera::cUIobj_Camera()
	: cUIobj_Component("Camera", eCOMPONENT_TYPE::CAMERA)
{
	SetStrID("CameraUI");
}

cUIobj_Camera::~cUIobj_Camera()
{
}


