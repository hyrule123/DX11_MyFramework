#pragma once
#include <Engine/CFSM.h>

#include "define_SCUnit.h"



//WPARAM : m_bIsBegin
//LPARAM : 미사용

class CScript_FSM_Attack_BeginEnd :
    public CFSM
{
public:
    CScript_FSM_Attack_BeginEnd(const string& _strKey);
    virtual ~CScript_FSM_Attack_BeginEnd();

    CScript_FSM_Attack_BeginEnd(const CScript_FSM_Attack_BeginEnd& _other);
    CLONE(CScript_FSM_Attack_BeginEnd);

public:
    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    virtual bool CheckCondition(UINT _eStateID, tEvent _tEventMsg) override;
    eFSM_RESULT BeginAttack() { return Transition(FSM_SCUnit::ATTACK_BEGIN_END, tEvent{ (DWORD_PTR)true, }); }
    eFSM_RESULT EndAttack() { return Transition(FSM_SCUnit::ATTACK_BEGIN_END, tEvent{ (DWORD_PTR)false, }); }

private:
    bool m_bIsBegin;

public:
    UINT m_uNextMoveID;
    tEvent m_tNextMoveMsg;

    void ResetNextMove() { m_uNextMoveID = FSM_SCUnit::END; m_tNextMoveMsg = {}; }
};

