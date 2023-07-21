#include "pch.h"
#include "cUIobj_AnimEditor.h"
#include "CUI_DragNDrop.h"

#include <Engine/cResMgr.h>
#include <Engine/cAnim2DAtlas.h>
#include <Engine/strKey_Default.h>

cUIobj_AnimEditor::cUIobj_AnimEditor()
	: cUI_BasicWindow("Animation Editor")
{
	SetSaveEnable(true);
}

cUIobj_AnimEditor::~cUIobj_AnimEditor()
{

}

void cUIobj_AnimEditor::init()
{
	m_AnimAtlasReceiver = new cUI_DragNDropReceiver(string(DIRECTORY_NAME::RES_ARR[(int)eRES_TYPE::ANIM2D_ATLAS]));
	AddChildUI(m_AnimAtlasReceiver);
}

void cUIobj_AnimEditor::Tick()
{
	tPtrData data = m_AnimAtlasReceiver->GetPtrData();
	if (nullptr != data.ptr)
	{
		cAnim2DAtlas* pAnimAtlas = (cAnim2DAtlas*)data.ptr;
		m_CurrentTarget = pAnimAtlas;
	}
}

void cUIobj_AnimEditor::render_update()
{
	if (nullptr == m_CurrentTarget)
		return;

	ImGui::Text("Current Texture: ");
	ImGui::SameLine();
	Ptr<cTexture> atlasTex = m_CurrentTarget->GetAtlasTex();
	
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
