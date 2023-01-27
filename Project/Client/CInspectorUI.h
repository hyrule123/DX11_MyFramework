#pragma once
#include "CUI.h"

class CComponentUI;

//Ư�� ���ӿ�����Ʈ�� ���� ���¸� Ȯ���ϴ� Ŭ����.

class CInspectorUI :
    public CUI
{
public:
    CInspectorUI();
    virtual ~CInspectorUI();

private:
    CGameObject* m_pTarget;
    CComponentUI* m_arrComUI[eCOMPONENT_END];

public:
    void tick() override;
    virtual int render_update() override;


private:


};

