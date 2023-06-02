#pragma once

#include <Engine/CFSM.h>

#include "define_SCUnit.h"

enum class eSCUNIT_ATTACK_LEVEL
{
    IDLE,
    BEGINATTACK,
    INATTACK,
    END
};

class CScript_FSM_Attack :
    public CFSM
{
public:
    CScript_FSM_Attack(const string& _strKey);
    virtual ~CScript_FSM_Attack();

    CScript_FSM_Attack(const CScript_FSM_Attack& _other) = default;
    CLONE(CScript_FSM_Attack);

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState, tEvent _tEventMsg = tEvent{}) override;

private:
    //Intantiate, 업그레이드 확인용으로 사용
    string m_strWeaponName;
    UINT m_uDefaultDamage;
    UINT m_uDamageAddedPerUpgrade;
    UINT m_uWeaponRange;

    bool m_bAttackBegin;
    int m_bReservedMove;
};
