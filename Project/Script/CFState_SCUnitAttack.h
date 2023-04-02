#pragma once

#include <Engine/CFState.h>

#include "define_SCUnit.h"

class CFState_SCUnitAttack :
    public CFState
{
public:
    CFState_SCUnitAttack();
    virtual ~CFState_SCUnitAttack();
    CLONE(CFState_SCUnitAttack);

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

};
