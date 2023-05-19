#pragma once

#include <Engine/CFSM.h>

#include "define_SCUnit.h"

class CFState_SCUnit_Attack :
    public CFSM
{
public:
    CFState_SCUnit_Attack();

    CFState_SCUnit_Attack(const CFState_SCUnit_Attack& _other) = default;
    CLONE(CFState_SCUnit_Attack);

    virtual ~CFState_SCUnit_Attack();

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override;

private:
    //Intantiate, 업그레이드 확인용으로 사용
    string m_strWeaponName;
    UINT m_uDefaultDamage;
    UINT m_uDamageAddedPerUpgrade;
    UINT m_uWeaponRange;

    int m_bReservedMove;
};
