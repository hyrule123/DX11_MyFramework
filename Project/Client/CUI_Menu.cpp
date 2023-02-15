
#include "pch.h"
#include "CUI_Menu.h"

CUI_Menu::CUI_Menu(const string& _MenuName)
	: CUI_BasicWindow("Menu")
	, m_bEnable(true)
{
	SetStrID(_MenuName);
}

CUI_Menu::~CUI_Menu()
{
}

bool CUI_Menu::beginUI()
{
	return ImGui::BeginMenu(GetStrID().data(), &m_bEnable);
}

void CUI_Menu::endUI()
{
	ImGui::EndMenu();
}