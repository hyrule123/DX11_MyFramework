#include "pch.h"
#include "cUIobj_Animator2D.h"

#include <Engine/cGameObject.h>

#include <Engine/cCom_Animator2D.h>
#include <Engine/EventDispatcher.h>



cUIobj_Animator2D::cUIobj_Animator2D()
	: cUIobj_Component("Animator2D", eCOMPONENT_TYPE::ANIMATOR)
{
	SetStrID("Animator2DUI");
}

cUIobj_Animator2D::~cUIobj_Animator2D()
{
}

void cUIobj_Animator2D::CreateNewComUI()
{
	if (ImGui::Button("Create New Animator2D"))
	{
		GetTargetObj()->AddComponent<cCom_Animator2D>();
	}
}

void cUIobj_Animator2D::EditComUI()
{
	if (ImGui::Button("Remove Animator2D"))
	{
		EventDispatcher::RemoveComponent(GetTargetObj(), eCOMPONENT_TYPE::ANIMATOR);
	}
}
