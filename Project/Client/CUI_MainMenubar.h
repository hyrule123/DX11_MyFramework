#pragma once
#include "CUI_Menubar.h"

class CUI_MenuItemGroup;

class CUI_MainMenubar :
    public CUI_Menubar
{
public:
    CUI_MainMenubar();
    virtual ~CUI_MainMenubar();

public:
    virtual void init() override;

private:
    CUI_MenuItemGroup* m_MenuItemGroup;

private:
    void DemoUI();
    void InspectorUI();
};

