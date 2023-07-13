#include "pch.h"
#include "CUIobj_AnimEditor.h"
#include "CUI_DragNDrop.h"

#include <Engine/CResMgr.h>
#include <Engine/CAnim2DAtlas.h>
#include <Engine/strKey_Default.h>

CUIobj_AnimEditor::CUIobj_AnimEditor()
	: CUI_BasicWindow("Animation Editor")
{
	SetSaveEnable(true);
}

CUIobj_AnimEditor::~CUIobj_AnimEditor()
{

}

void CUIobj_AnimEditor::init()
{
	m_AnimAtlasReceiver = new CUI_DragNDropReceiver(string(DIRECTORY_NAME::RES_ARR[(int)eRES_TYPE::ANIM2D_ATLAS]));
	AddChildUI(m_AnimAtlasReceiver);
}

void CUIobj_AnimEditor::tick()
{
	tPtrData data = m_AnimAtlasReceiver->GetPtrData();
	if (nullptr != data.ptr)
	{
		CAnim2DAtlas* pAnimAtlas = (CAnim2DAtlas*)data.ptr;
		m_CurrentTarget = pAnimAtlas;
	}
}

void CUIobj_AnimEditor::render_update()
{
	if (nullptr == m_CurrentTarget)
		return;

	ImGui::Text("Current Texture: ");
	ImGui::SameLine();
	Ptr<CTexture> atlasTex = m_CurrentTarget->GetAtlasTex();
	
	if (nullptr == atlasTex)
		ImGui::Text("None");
	else
		ImGui::Text(atlasTex->GetKey().c_str());
	
	ImGui::Text("[Drop Texture Here To ]");


	if(ImGui::Button("Unload Atlas"))
	{
		m_CurrentTarget = nullptr;
	}
}
