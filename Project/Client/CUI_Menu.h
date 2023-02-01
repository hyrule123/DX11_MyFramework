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
	//������ ���̾ƿ� ��ġ �۾��� ���⼭ ����. �Ʒ��� render_update�� ȣ���Ѵ�.
	//finaltick()�� ������ ���� �ƿ� �ٸ� ������� �����ؾ��� ���� �ֱ� ������ virtual ���·� �д�.
	virtual void finaltick() final;

	//�޴��ٿ� ���� ������ �� ���� �������ؼ� �߰��ϸ� ��.
	virtual int render_update() override { return TRUE; };

private:
	bool m_bEnable;
};

