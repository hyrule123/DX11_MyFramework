#pragma once

#include <Engine/CFSM.h>

class CScript_FSM_AttackBegin :
    public CFSM
{
public:
    CScript_FSM_AttackBegin(const string& _strKey);

    CScript_FSM_AttackBegin(const CScript_FSM_AttackBegin& _other) = default;
    CLONE(CScript_FSM_AttackBegin);

    virtual ~CScript_FSM_AttackBegin();
    

public:
    virtual bool CheckCondition(UINT _eStateID, tEvent _tEventMsg = tEvent{}) override;

    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;
};

