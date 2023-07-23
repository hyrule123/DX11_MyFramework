#include "pch.h"
#include "cUIobj_Collider2D.h"
#include "cUI_ComboBox.h"

#include "macroFunc_Imgui.h"

#include <Engine/cGameObject.h>
#include <Engine/cCom_Coll2D_Rect.h>
#include <Engine/cCom_Coll2D_Point.h>
#include <Engine/cCom_Coll2D_Circle.h>

#include <Engine/EventDispatcher.h>

#include <Script/SC_Func.h>

cUIobj_Collider2D::cUIobj_Collider2D()
	: cUIobj_Component("CCollider2D", eCOMPONENT_TYPE::COLLIDER)
	, m_ComboColTypeSelector()
{
	SetStrID("Collider2D");
}

cUIobj_Collider2D::~cUIobj_Collider2D()
{

}

void cUIobj_Collider2D::init()
{
	m_ComboColTypeSelector = new cUI_ComboBox("Select Collider Type");
	AddChildUI(m_ComboColTypeSelector);

	//충돌체 타입을 tPtrData에 저장해서 사용
	m_ComboColTypeSelector->AddItem(tComboItem{ "Rect", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::RECT)});
	m_ComboColTypeSelector->AddItem(tComboItem{ "Circle", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::CIRCLE)});
	m_ComboColTypeSelector->AddItem(tComboItem{ "OBB", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::OBB)});
	m_ComboColTypeSelector->AddItem(tComboItem{ "Point", tPtrData(nullptr, (size_t)eCOLLIDER_TYPE_2D::POINT)});
	
	//m_ComboColTypeSelector->AddItem(tComboItem{ "Point", });
}

void cUIobj_Collider2D::Tick()
{
}

void cUIobj_Collider2D::CreateNewComUI()
{
	m_ComboColTypeSelector->SetActive(true);

	//이 버튼을 누를 경우 새 충돌체를 생성한다.
	if (ImGui::Button("Create New Collider"))
	{
		const tComboItem& Selected = m_ComboColTypeSelector->GetCurrentSelected();
		
		//충돌체 타입을 tPtrData에 저장해서 사용
		switch ((eCOLLIDER_TYPE_2D)Selected.pData.size)
		{
		case eCOLLIDER_TYPE_2D::RECT:
			GetTargetObj()->AddComponent(new cCom_Coll2D_Rect);
			break;
		case eCOLLIDER_TYPE_2D::CIRCLE:
			GetTargetObj()->AddComponent(new cCom_Coll2D_Circle);
			break;
		case eCOLLIDER_TYPE_2D::OBB:
			ERROR_MESSAGE_A("Not Supported");
			break;
		case eCOLLIDER_TYPE_2D::POINT:
			ERROR_MESSAGE_A("Not Supported");
			break;
		case eCOLLIDER_TYPE_2D::END:
			ERROR_MESSAGE_A("Not Supported");
			break;
		default:
			break;
		}
	}
}

void cUIobj_Collider2D::EditComUI()
{
	m_ComboColTypeSelector->SetActive(false);

	eCOLLIDER_TYPE_2D type = GetTargetObj()->GetComponent<ICollider2D>()->GetcColliderType();

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
		EventDispatcher::RemoveComponent(GetTargetObj(), eCOMPONENT_TYPE::COLLIDER);
	}
}

void cUIobj_Collider2D::RectEditUI()
{
	cCom_Coll2D_Rect* pRect = static_cast<cCom_Coll2D_Rect*>(GetTargetObj()->GetComponent<ICollider2D>());

	IMGUI_AlignedText("Collider Size: ");
	
	Vec2 v2RectSize = pRect->GetCollSize().XY();
	if (ImGui::DragFloat2("##CollSize", v2RectSize))
	{
		if (v2RectSize.x > 10.f && v2RectSize.y > 10.f)
		{
			pRect->SetCollSize(v2RectSize);
		}
	}

	ImGui::NewLine();
	ImGui::TextColored(ImColorPreset::Orange, "[Set Building Size]");

	IMGUI_AlignedText("Mega Tile Size: ");
	static int BuildingSize[2] = {};
	ImGui::InputInt2("##BuildingSizeMegaTile", BuildingSize);

	static Vec4 Offset;
	IMGUI_AlignedText("Offsets(Vec4)");
	ImGui::InputFloat4("##BuildingSizeOffset", Offset);

	if (ImGui::Button("Apply"))
	{
		if(0 < BuildingSize[0]&& 0 < BuildingSize[1])
			SC_Func::SetSCBuildingSize(pRect, BuildingSize[0], BuildingSize[1], Offset);
		else
		{
			ERROR_MESSAGE_A("Mega tile numbers must be more than 1");
		}
	}
	
}

void cUIobj_Collider2D::CircleEditUI()
{
	cCom_Coll2D_Circle* pCol = static_cast<cCom_Coll2D_Circle*>(GetTargetObj()->GetComponent<ICollider2D>());
	float fRad = pCol->GetRadius();
	
	IMGUI_AlignedText("Radius : ");
	if (ImGui::DragFloat("##SetRadius", &fRad))
	{
		if(fRad > 0.f)
			pCol->SetRadius(fRad);
	}
}

//void cUIobj_Collider2D::render_update()
//{
//	ImGui::NewLine();
//
//	IMGUI_AlignedText("ColliderType");
//}

