#pragma once
#include "cUI_BasicWindow.h"

class cUIobj_Component;
class cGameObject;

//특정 게임오브젝트의 현재 상태를 확인하는 클래스.

class cUIobj_Inspector :
    public cUI_BasicWindow
{
public:
    cUIobj_Inspector();
    virtual ~cUIobj_Inspector();

public:
    void init() override;
    void Tick() override;

    void render_update() override;

private:
    virtual void Save(Json::Value& _Save) override;
    virtual void Load(Json::Value& _Load) override;

private:
    cGameObject* m_pTarget;
    cUIobj_Component* m_arrComUI[(UINT)eCOMPONENT_TYPE::END];

public:
    void SetTarget(cGameObject* _pTarget);

private:
    void TransformIndicator();
};
