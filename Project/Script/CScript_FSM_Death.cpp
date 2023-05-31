#include "pch.h"
#include "CScript_FSM_Death.h"

#include "define_SCUnit.h"

CScript_FSM_Death::CScript_FSM_Death(const string& _strKey)
    :CFSM(_strKey, FSM_SCUnit::DEATH)
{
}


CScript_FSM_Death::~CScript_FSM_Death()
{
}

void CScript_FSM_Death::EnterState()
{
}

void CScript_FSM_Death::OnState()
{
}

void CScript_FSM_Death::EndState()
{
}

bool CScript_FSM_Death::CheckCondition(UINT _eState, tEvent _tEventMsg)
{
    return false;
}
