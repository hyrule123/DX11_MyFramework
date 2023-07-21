#include "pch.h"
#include "cUI_Menubar.h"

#include "cUI_Menu.h"
#include "cUI_MenuItem.h"

#include <Engine/CKeyMgr.h>
#include <Engine/cEngine.h>


cUI_Menubar::cUI_Menubar(const string& _strName, bool _bIsMainMenubar)
	: cUI_BasicWindow(_strName)
    , m_bMainMenuBar(_bIsMainMenubar)
{
}

cUI_Menubar::~cUI_Menubar()
{
}

bool cUI_Menubar::beginUI()
{
    //메뉴바 설정에 따라 다른 함수 호출
    if (true == m_bMainMenuBar)
    {
        return ImGui::BeginMainMenuBar();
    }
        
    else
    {
        return ImGui::BeginMenuBar();
    }
}

void cUI_Menubar::render_update()
{

}

void cUI_Menubar::endUI()
{
    if (true == m_bMainMenuBar)
        ImGui::EndMainMenuBar();
    else
        ImGui::EndMenuBar();
}

cUI_Menu* cUI_Menubar::AddMenu(const string& _strName)
{
    cUI_Menu* menu = new cUI_Menu(_strName);
    AddChildUI(menu);

    return menu;
}

cUI_MenuItem* cUI_Menubar::AddMenuItem(const string& _MenuName, const string& _MenuItemName, tPtrData _pData)
{
    cUI_Menu* pMenu = dynamic_cast<cUI_Menu*>(FindChildUIByName(_MenuName));
    if (nullptr == pMenu)
        return nullptr;

    cUI_MenuItem* pMenuItem = pMenu->AddMenuItem(_MenuItemName, _pData);

    return pMenuItem;
}



