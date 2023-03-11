#include "pch.h"
#include "CUI_Menubar.h"

#include "CUI_Menu.h"
#include "CUI_MenuItem.h"


CUI_Menubar::CUI_Menubar(const string& _strName, bool _bIsMainMenubar)
	: CUI_BasicWindow(_strName)
    , m_bMainMenuBar(_bIsMainMenubar)
{
}

CUI_Menubar::~CUI_Menubar()
{
}

bool CUI_Menubar::beginUI()
{
    //메뉴바 설정에 따라 다른 함수 호출
    if (true == m_bMainMenuBar)
        return ImGui::BeginMainMenuBar();
    else
    {
        return ImGui::BeginMenuBar();
    }
}

void CUI_Menubar::endUI()
{
    if (true == m_bMainMenuBar)
        ImGui::EndMainMenuBar();
    else
        ImGui::EndMenuBar();
}

CUI_Menu* CUI_Menubar::AddMenu(const string& _strName)
{
    CUI_Menu* menu = new CUI_Menu(_strName);
    AddChildUI(menu);

    return menu;
}

CUI_MenuItem* CUI_Menubar::AddMenuItem(const string& _MenuName, const string& _MenuItemName, tDataPtr _pData)
{
    CUI_Menu* pMenu = dynamic_cast<CUI_Menu*>(FindChildUIByName(_MenuName));
    if (nullptr == pMenu)
        return nullptr;

    CUI_MenuItem* pMenuItem = pMenu->AddMenuItem(_MenuItemName, _pData);

    return pMenuItem;
}



