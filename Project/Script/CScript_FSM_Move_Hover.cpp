#include "pch.h"
#include "CScript_FSM_Move_Hover.h"

#include "define_SCUnit.h"

CScript_FSM_Move_Hover::CScript_FSM_Move_Hover(const string& _strKey)
    : CFSM(_strKey, FSM_SCUnit::MOVE)
{
}

CScript_FSM_Move_Hover::~CScript_FSM_Move_Hover()
{
}

CScript_FSM_Move_Hover::CScript_FSM_Move_Hover(const CScript_FSM_Move_Hover& _other)
    : CFSM(_other)
{
}

bool CScript_FSM_Move_Hover::CheckCondition(UINT _eState, tEvent _tEventMsg)
{
    return false;
}

void CScript_FSM_Move_Hover::EnterState()
{
}

void CScript_FSM_Move_Hover::OnState()
{
}

void CScript_FSM_Move_Hover::EndState()
{
}
