
#include "pch.h"
#include "CUI_Menu.h"

#include "CUI_MenuItem.h"

CUI_Menu::CUI_Menu(const string& _MenuName)
	: CUI_BasicWindow(_MenuName)
	, m_bEnable(true)
{
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

CUI_MenuItem* CUI_Menu::AddMenuItem(const string& _strName, tDataPtr _pData)
{
	CUI_MenuItem* pMenuItem = new CUI_MenuItem(_strName, _pData);

	AddChildUI(pMenuItem);

	return pMenuItem;
}
