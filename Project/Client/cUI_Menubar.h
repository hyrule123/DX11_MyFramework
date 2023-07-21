#pragma once
#include "cUI_BasicWindow.h"

class cUI_Menu;
class cUI_MenuItem;

class cUI_Menubar :
    public cUI_BasicWindow
{
	cUI_Menubar() = delete;

public:
    cUI_Menubar(const string& _strName, bool _bIsMainMenubar = false);
    virtual ~cUI_Menubar();

public:
	virtual bool beginUI() override;
	virtual void render_update() override;
	virtual void endUI() override;

private:
	bool m_bMainMenuBar;

public:
	cUI_Menu* AddMenu(const string& _strName);
	cUI_MenuItem* AddMenuItem(const string& _MenuName, const string& _MenuItemName, tPtrData _pData);
};

