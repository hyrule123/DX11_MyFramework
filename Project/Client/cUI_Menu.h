#pragma once
#include "cUI_Widget.h"

class cUI_MenuItem;

class cUI_Menu :
    public cUI_BasicWindow
{
public:
	cUI_Menu() = delete;
public:
    cUI_Menu(const string& _MenuName);
    virtual ~cUI_Menu();

public:
	//설정된 레이아웃 배치 작업은 여기서 진행. 아래의 render_update를 호출한다.
	//FinalTick()은 위젯에 따라서 아예 다른 방식으로 정의해야할 때도 있기 때문에 virtual 형태로 둔다.
	virtual bool beginUI() override;
	virtual void endUI() override;

private:
	bool m_bEnable;

public:
	cUI_MenuItem* AddMenuItem(const string& _strName, tPtrData _pData = tPtrData{});
};

