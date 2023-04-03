#include "pch.h"
#include "CUIobj_Animator2D.h"

#include <Engine/CAnimator2D.h>
#include <Engine/EventDispatcher.h>


CUIobj_Animator2D::CUIobj_Animator2D()
	: CUIobj_Component("Animator2D", eCOMPONENT_TYPE::ANIMATOR2D)
{
	SetStrID("Animator2DUI");
}

CUIobj_Animator2D::~CUIobj_Animator2D()
{
}

void CUIobj_Animator2D::CreateNewComUI()
{
	if (ImGui::Button("Create New Animator2D"))
	{
		GetTargetObj()->AddComponent(new CAnimator2D);
	}
}

void CUIobj_Animator2D::EditComUI()
{
	if (ImGui::Button("Remove Animator2D"))
	{
		EventDispatcher::RemoveComponent(GetTargetObj(), eCOMPONENT_TYPE::ANIMATOR2D);
	}
}
