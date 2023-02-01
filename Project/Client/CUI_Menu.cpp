
#include "pch.h"
#include "CUI_Menu.h"

CUI_Menu::CUI_Menu(const string& _MenuName)
	: CUI("##Menu")
	, m_bEnable(true)
{
	SetName(_MenuName);
}

CUI_Menu::~CUI_Menu()
{
}

void CUI_Menu::finaltick()
{
	if (ImGui::BeginMenu(GetName().data(), &m_bEnable))
	{
		render_update();
		LoopChildFinaltick();
		
		ImGui::EndMenu();
	}
}
