#pragma once

#include <Engine/IScript.h>

class cGameObject;
class cScript_MouseCursor :
    public IScript
{
public:
    cScript_MouseCursor();
    virtual ~cScript_MouseCursor();

    CLONE(cScript_MouseCursor);

private:
    Vec2 m_v2CursorPos;

    //Z축 체크용
    cGameObject* m_pObj_ZCheck;

    std::function<void(cGameObject*)> m_arrpFuncLBTNCallback[(int)eKEY_STATE::END];
    std::function<void(cGameObject*)> m_arrpFuncRBTNCallback[(int)eKEY_STATE::END];

    cGameObject* m_pSelectedGameObject;

public:
    void AddFuncLBTNCallback(eKEY_STATE _eMouseAction, std::function<void(cGameObject*)> _pFunc) { m_arrpFuncLBTNCallback[(int)_eMouseAction] = _pFunc; }

    void AddFuncRBTNCallback(eKEY_STATE _eMouseAction, std::function<void(cGameObject*)> _pFunc) { m_arrpFuncRBTNCallback[(int)_eMouseAction] = _pFunc; }

    GETTER(cGameObject*, m_pSelectedGameObject, SelectedObject);

public:
    virtual void Tick() override;
    virtual void OnCollision(ICollider* _Collider, const Vec3& _v3HitPoint) override;


private:
    void CalcPos();
};
