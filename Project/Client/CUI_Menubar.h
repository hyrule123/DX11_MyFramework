#pragma once
#include "CUI.h"

struct tMenu
{
	string sLabel;
	bool	bEnable;
};

class CUI_Menubar :
    public CUI
{
	CUI_Menubar() = delete;

public:
    CUI_Menubar(bool _bIsMainMenubar = false);
    virtual ~CUI_Menubar();

public:

	//설정된 레이아웃 배치 작업은 여기서 진행. 아래의 render_update를 호출한다.
	//finaltick()은 위젯에 따라서 아예 다른 방식으로 정의해야할 때도 있기 때문에 virtual 형태로 둔다.
	virtual void finaltick() final;

	
	//메뉴바에 넣을 값들은 이 값을 재정의해서 추가하면 됨.
	virtual int render_update() = 0;

private:
	bool m_bMainMenuBar;
};

