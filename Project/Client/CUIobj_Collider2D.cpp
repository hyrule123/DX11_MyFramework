#include "pch.h"
#include "CUIobj_Collider2D.h"
#include "CUI_ComboBox.h"

#include "macroFunc_Imgui.h"

#include <Engine/CGameObject.h>
#include <Engine/CCollider2D_Rect.h>
#include <Engine/CCollider2D_Point.h>
#include <Engine/CCollider2D_Circle.h>

#include <Engine/EventDispatcher.h>


CUIobj_Collider2D::CUIobj_Collider2D()
	: CUIobj_Component("CCollider2D", eCOMPONENT_TYPE::COLLIDER2D)
	, m_ComboColTypeSelector()
{
	SetStrID("Collider2D");
}

CUIobj_Collider2D::~CUIobj_Collider2D()
{

}

void CUIobj_Collider2D::init()
{
	//항상 표시
	m_ComboColTypeSelector = new CUI_ComboBox("Select Collider Type");
	AddChildUI(m_ComboColTypeSelector);

	//충돌체 타입을 tPtrData에 저장해서 사용
	m_ComboColTypeSelector->AddItem(tComboItem{ "Rect", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::RECT)});
	m_ComboColTypeSelector->AddItem(tComboItem{ "Circle", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::CIRCLE)});
	m_ComboColTypeSelector->AddItem(tComboItem{ "OBB", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::OBB)});
	m_ComboColTypeSelector->AddItem(tComboItem{ "Point", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::POINT)});
	
	//m_ComboColTypeSelector->AddItem(tComboItem{ "Point", });
}

void CUIobj_Collider2D::tick()
{
}

void CUIobj_Collider2D::CreateNewComUI()
{
	//이 버튼을 누를 경우 새 충돌체를 생성한다.
	if (ImGui::Button("Create New Collider"))
	{
		const tComboItem& Selected = m_ComboColTypeSelector->GetCurrentSelected();
		
		//충돌체 타입을 tPtrData에 저장해서 사용
		switch ((eCOLLIDER_TYPE_2D)Selected.pData.size)
		{
		case eCOLLIDER_TYPE_2D::RECT:
			GetTargetObj()->AddComponent(new CCollider2D_Rect);
			break;
		case eCOLLIDER_TYPE_2D::CIRCLE:
			GetTargetObj()->AddComponent(new CCollider2D_Circle);
			break;
		case eCOLLIDER_TYPE_2D::OBB:
			ERROR_MESSAGE(Not Supported);
			break;
		case eCOLLIDER_TYPE_2D::POINT:
			ERROR_MESSAGE(Not Supported);
			break;
		case eCOLLIDER_TYPE_2D::END:
			ERROR_MESSAGE(Not Supported);
			break;
		default:
			break;
		}
	}
}

void CUIobj_Collider2D::EditComUI()
{
	eCOLLIDER_TYPE_2D type = GetTargetObj()->Collider2D()->GetColliderType();

	switch (type)
	{
	case eCOLLIDER_TYPE_2D::RECT:
		RectEditUI();
		break;
	case eCOLLIDER_TYPE_2D::CIRCLE:
		CircleEditUI();
		break;
	case eCOLLIDER_TYPE_2D::OBB:
		break;
	case eCOLLIDER_TYPE_2D::POINT:
		break;
	case eCOLLIDER_TYPE_2D::END:
		break;
	default:
		break;
	}



	//충돌체 제거 버튼을 눌렀을 경우
	if (ImGui::Button("Remove Collider"))
	{
		//여기 들어왔다는 건 충돌체 컴포넌트가 있다는 뜻이므로 검사할 필요 없이 바로 등록하면 됨.
		EventDispatcher::RemoveComponent(GetTargetObj(), eCOMPONENT_TYPE::COLLIDER2D);
	}
}

void CUIobj_Collider2D::RectEditUI()
{
	CCollider2D_Rect* pRect = static_cast<CCollider2D_Rect*>(GetTargetObj()->Collider2D());

	IMGUI_AlignedText("Collider Size: ");
	
	//Vec2 v2RectSize = pRect->Getv2RectSize();
	//if (ImGui::DragFloat2("##CollSize", v2RectSize))
	//{
	//	pRect->Setv2RectSize(v2RectSize);
	//}

}

void CUIobj_Collider2D::CircleEditUI()
{
}

//void CUIobj_Collider2D::render_update()
//{
//	ImGui::NewLine();
//
//	IMGUI_AlignedText("ColliderType");
//}

