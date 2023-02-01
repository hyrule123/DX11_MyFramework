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
    virtual int render_update() override;

private:

};

