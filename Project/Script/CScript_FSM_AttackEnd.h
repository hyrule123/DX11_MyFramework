#pragma once

#include <Engine/CFSM.h>

class CScript_FSM_AttackEnd :
    public CFSM
{
public:
    CScript_FSM_AttackEnd(const string& _strKey);
    virtual ~CScript_FSM_AttackEnd();
    CLONE(CScript_FSM_AttackEnd);

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState, tEvent _tEventMsg) override;

private:
    bool m_bCurMotionEnd;
    UINT m_ReservedMotion;
};

