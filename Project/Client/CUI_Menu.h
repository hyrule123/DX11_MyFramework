#pragma once
#include "CUI_Widget.h"


class CUI_Menu :
    public CUI
{
public:
	CUI_Menu() = delete;
public:
    CUI_Menu(const string& _MenuName);
    virtual ~CUI_Menu();

public:
	//설정된 레이아웃 배치 작업은 여기서 진행. 아래의 render_update를 호출한다.
	//finaltick()은 위젯에 따라서 아예 다른 방식으로 정의해야할 때도 있기 때문에 virtual 형태로 둔다.
	virtual void finaltick() final;

	//메뉴바에 넣을 값들은 이 값을 재정의해서 추가하면 됨.
	virtual int render_update() override { return TRUE; };

private:
	bool m_bEnable;
};

