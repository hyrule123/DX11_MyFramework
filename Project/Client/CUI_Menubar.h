#pragma once
#include "CUI_BasicWindow.h"

class CUI_Menu;
class CUI_MenuItem;

class CUI_Menubar :
    public CUI_BasicWindow
{
	CUI_Menubar() = delete;

public:
    CUI_Menubar(const string& _strName, bool _bIsMainMenubar = false);
    virtual ~CUI_Menubar();

public:
	virtual bool beginUI() override;
	virtual void endUI() override;

private:
	bool m_bMainMenuBar;

public:
	CUI_Menu* AddMenu(const string& _strName);
	CUI_MenuItem* AddMenuItem(const string& _MenuName, const string& _MenuItemName, DWORD_PTR _pData);
};

