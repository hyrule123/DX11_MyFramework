#pragma once
#include "CUI_BasicWindow.h"

class CUI_Component;

//특정 게임오브젝트의 현재 상태를 확인하는 클래스.

class CUI_Inspector :
    public CUI_BasicWindow
{
public:
    CUI_Inspector();
    virtual ~CUI_Inspector();

public:
    void init() override;
    void tick() override;

private:
    CGameObject* m_pTarget;
    CUI_Component* m_arrComUI[(UINT)eCOMPONENT_TYPE::END];

public:
    void SetTarget(CGameObject* _pTarget);



private:
    virtual void Save(Json::Value& _Save) override;
    virtual void Load(Json::Value& _Load) override;

};