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

	//������ ���̾ƿ� ��ġ �۾��� ���⼭ ����. �Ʒ��� render_update�� ȣ���Ѵ�.
	//finaltick()�� ������ ���� �ƿ� �ٸ� ������� �����ؾ��� ���� �ֱ� ������ virtual ���·� �д�.
	virtual void finaltick() final;

	
	//�޴��ٿ� ���� ������ �� ���� �������ؼ� �߰��ϸ� ��.
	virtual int render_update() = 0;

private:
	bool m_bMainMenuBar;
};

