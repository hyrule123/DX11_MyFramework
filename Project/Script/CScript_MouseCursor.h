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
    std::function<void(CGameObject*)> m_arrpFuncLBTNCallback[(int)eKEY_STATE::END];
    std::function<void(CGameObject*)> m_arrpFuncRBTNCallback[(int)eKEY_STATE::END];

    CGameObject* m_pSelectedGameObject;

public:
    void AddFuncLBTNCallback(eKEY_STATE _eMouseAction, std::function<void(CGameObject*)> _pFunc) { m_arrpFuncLBTNCallback[(int)_eMouseAction] = _pFunc; }

    void AddFuncRBTNCallback(eKEY_STATE _eMouseAction, std::function<void(CGameObject*)> _pFunc) { m_arrpFuncRBTNCallback[(int)_eMouseAction] = _pFunc; }

public:
    virtual void tick() override;
    virtual void OnCollision(CCollider* _Collider) override;
};
