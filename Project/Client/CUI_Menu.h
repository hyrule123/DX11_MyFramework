#pragma once
#include "CUI_Widget.h"

class CUI_MenuItem;

class CUI_Menu :
    public CUI_BasicWindow
{
public:
	CUI_Menu() = delete;
public:
    CUI_Menu(const string& _MenuName);
    virtual ~CUI_Menu();

public:
	//설정된 레이아웃 배치 작업은 여기서 진행. 아래의 render_update를 호출한다.
	//finaltick()은 위젯에 따라서 아예 다른 방식으로 정의해야할 때도 있기 때문에 virtual 형태로 둔다.
	virtual bool beginUI() override;
	virtual void endUI() override;

private:
	bool m_bEnable;

public:
	CUI_MenuItem* AddMenuItem(const string& _strName, DWORD_PTR _pData = (DWORD_PTR)0);
};

