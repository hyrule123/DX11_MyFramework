#include "pch.h"
#include "CScript_FSM_Move_Hover.h"

#include "define_SC.h"

CScript_FSM_Move_Hover::CScript_FSM_Move_Hover(const string& _strKey)
    : CFSM(_strKey, SC::FSM::MOVE)
{
}

CScript_FSM_Move_Hover::~CScript_FSM_Move_Hover()
{
}

CScript_FSM_Move_Hover::CScript_FSM_Move_Hover(const CScript_FSM_Move_Hover& _other)
    : CFSM(_other)
{
}

eFSM_RESULT CScript_FSM_Move_Hover::CheckCondition(const tFSM_Event& _tEvent)
{
    return eFSM_RESULT::REJECT;
}

void CScript_FSM_Move_Hover::EnterState(const tFSM_Event& _tEvent)
{
}

void CScript_FSM_Move_Hover::OnState()
{
}

void CScript_FSM_Move_Hover::EndState()
{
}
