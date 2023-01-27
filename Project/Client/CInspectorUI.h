#pragma once
#include "CUI.h"

class CComponentUI;

//특정 게임오브젝트의 현재 상태를 확인하는 클래스.

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

