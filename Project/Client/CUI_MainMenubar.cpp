#include "pch.h"
#include "CUI_MainMenubar.h"

#include "CUI_MenuItemGroup.h"
#include "CUI_Menu.h"

#include "CImGuiMgr.h"

CUI_MainMenubar::CUI_MainMenubar()
    : CUI_Menubar(true)
    , m_MenuItemGroup()
{
    CUI_Menu* WindowsMenu = new CUI_Menu("Open Window");
    AddChildUI(WindowsMenu);
    
    m_MenuItemGroup = new CUI_MenuItemGroup;
    WindowsMenu->AddChildUI(m_MenuItemGroup);


    tMenuItem Item;
    Item.Label = "Demo";
    Item.Callback = std::bind(&CUI_MainMenubar::DemoUI, this);
    m_MenuItemGroup->AddItem(Item);

    Item.Label = "Inspector";
    Item.Callback = std::bind(&CUI_MainMenubar::InspectorUI, this);
    m_MenuItemGroup->AddItem(Item);
}

CUI_MainMenubar::~CUI_MainMenubar()
{
}

void CUI_MainMenubar::init()
{
}

void CUI_MainMenubar::DemoUI()
{
    CImGuiMgr::GetInst()->ToggleDemoWindow();
}

void CUI_MainMenubar::InspectorUI()
{
    CUI* pUI = CImGuiMgr::GetInst()->FindUI("Inspector");
    if (nullptr != pUI)
    {
        pUI->ToggleActive();
    }
}
