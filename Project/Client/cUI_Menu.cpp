
#include "pch.h"
#include "cUI_Menu.h"

#include "cUI_MenuItem.h"

cUI_Menu::cUI_Menu(const string& _MenuName)
	: cUI_BasicWindow(_MenuName)
	, m_bEnable(true)
{
}

cUI_Menu::~cUI_Menu()
{
}

bool cUI_Menu::beginUI()
{
	return ImGui::BeginMenu(GetStrID().data(), &m_bEnable);
}

void cUI_Menu::endUI()
{
	ImGui::EndMenu();
}

cUI_MenuItem* cUI_Menu::AddMenuItem(const string& _strName, tPtrData _pData)
{
	cUI_MenuItem* pMenuItem = new cUI_MenuItem(_strName, _pData);

	AddChildUI(pMenuItem);

	return pMenuItem;
}
