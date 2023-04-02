#pragma once

#include <Engine/CScript.h>

class CGameObject;
class CScript_MouseCursor :
    public CScript
{
public:
    CScript_MouseCursor();
    virtual ~CScript_MouseCursor();
    CLONE(CScript_MouseCursor);

private:
    Vec2 m_v2CursorPos;

    //Z축 체크용
    CGameObject* m_pObj_ZCheck;

    std::function<void(CGameObject*)> m_arrpFuncLBTNCallback[(int)eKEY_STATE::END];
    std::function<void(CGameObject*)> m_arrpFuncRBTNCallback[(int)eKEY_STATE::END];

    CGameObject* m_pSelectedGameObject;

public:
    void AddFuncLBTNCallback(eKEY_STATE _eMouseAction, std::function<void(CGameObject*)> _pFunc) { m_arrpFuncLBTNCallback[(int)_eMouseAction] = _pFunc; }

    void AddFuncRBTNCallback(eKEY_STATE _eMouseAction, std::function<void(CGameObject*)> _pFunc) { m_arrpFuncRBTNCallback[(int)_eMouseAction] = _pFunc; }

    GETTER(CGameObject*, m_pSelectedGameObject, SelectedObject);

public:
    virtual void tick() override;
    virtual void OnCollision(CCollider* _Collider) override;

private:
    void CalcPos();
};
