#pragma once
#include <Engine/CFSM.h>


class CScript_FSM_Attack_BeginEnd :
    public CFSM
{
public:
    CScript_FSM_Attack_BeginEnd();
    virtual ~CScript_FSM_Attack_BeginEnd();

    CScript_FSM_Attack_BeginEnd(const CScript_FSM_Attack_BeginEnd& _other);
    CLONE(CScript_FSM_Attack_BeginEnd);

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    virtual bool CheckCondition(UINT _eStateID, tEvent _tEventMsg) override;


};

