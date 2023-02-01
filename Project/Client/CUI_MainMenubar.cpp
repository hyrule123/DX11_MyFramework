#include "pch.h"
#include "CUI_MainMenubar.h"

#include "CUI_MenuItemGroup.h"
#include "CUI_Menu.h"

CUI_MainMenubar::CUI_MainMenubar()
    : CUI_Menubar(true)
{
    CUI_Menu* WindowsMenu = new CUI_Menu("Windows");
    AddChildUI(WindowsMenu);
    
    CUI_MenuItemGroup* Group = new CUI_MenuItemGroup;
    WindowsMenu->AddChildUI(Group);

    tMenuItem Item("InspectorUI");
    Group->AddItem(Item);
}

CUI_MainMenubar::~CUI_MainMenubar()
{
}

int CUI_MainMenubar::render_update()
{

    return TRUE;
}
