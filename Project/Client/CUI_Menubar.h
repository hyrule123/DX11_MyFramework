#pragma once
#include "CUI_BasicWindow.h"

struct tMenu
{
	string sLabel;
	bool	bEnable;
};

class CUI_Menubar :
    public CUI_BasicWindow
{
	CUI_Menubar() = delete;

public:
    CUI_Menubar(bool _bIsMainMenubar = false);
    virtual ~CUI_Menubar();

public:
	virtual bool beginUI() override;
	virtual void endUI() override;


private:
	bool m_bMainMenuBar;
};

