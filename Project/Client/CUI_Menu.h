#pragma once
#include "CUI_Widget.h"


class CUI_Menu :
    public CUI_BasicWindow
{
public:
	CUI_Menu() = delete;
public:
    CUI_Menu(const string& _MenuName);
    virtual ~CUI_Menu();

public:
	//������ ���̾ƿ� ��ġ �۾��� ���⼭ ����. �Ʒ��� render_update�� ȣ���Ѵ�.
	//finaltick()�� ������ ���� �ƿ� �ٸ� ������� �����ؾ��� ���� �ֱ� ������ virtual ���·� �д�.
	virtual bool beginUI() override;
	virtual void endUI() override;

private:
	bool m_bEnable;
};

