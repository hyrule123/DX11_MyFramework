#pragma once
#include "CUI_BasicWindow.h"

class CUIobj_Component;

//특정 게임오브젝트의 현재 상태를 확인하는 클래스.

class CUIobj_Inspector :
    public CUI_BasicWindow
{
public:
    CUIobj_Inspector();
    virtual ~CUIobj_Inspector();

public:
    void init() override;
    void tick() override;

    void render_update() override;

private:
    CGameObject* m_pTarget;
    CUIobj_Component* m_arrComUI[(UINT)eCOMPONENT_TYPE::END];

public:
    void SetTarget(CGameObject* _pTarget);



private:
    virtual void Save(Json::Value& _Save) override;
    virtual void Load(Json::Value& _Load) override;

};
