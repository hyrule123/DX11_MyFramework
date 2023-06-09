#pragma once
#include <Engine/CFSM.h>

#include "define_SCUnit.h"

//srcParam : X
//destParam : (bool)m_bIsEnd

class CScript_FSM_Attack_BeginEnd :
    public CFSM
{
public:
    CScript_FSM_Attack_BeginEnd(const string& _strKey);
    virtual ~CScript_FSM_Attack_BeginEnd();

    CScript_FSM_Attack_BeginEnd(const CScript_FSM_Attack_BeginEnd& _other);
    CLONE(CScript_FSM_Attack_BeginEnd);

public:
    virtual void EnterState(const tFSM_Event& _tEvent) override;
    virtual void OnState() override;
    virtual void EndState() override;

    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent) override;
    

private:
    bool m_bIsEnd;
    void SetBeginEnd(bool _bIsEnd) { m_bIsEnd = _bIsEnd; }

};

