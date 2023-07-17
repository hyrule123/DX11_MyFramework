#include "pch.h"
#include "cScript_FSM_Move_Hover.h"

#include "define_SC.h"

cScript_FSM_Move_Hover::cScript_FSM_Move_Hover(const string_view _strKey)
    : I_FSM(_strKey, SC::FSM::MOVE)
{
}

cScript_FSM_Move_Hover::~cScript_FSM_Move_Hover()
{
}

cScript_FSM_Move_Hover::cScript_FSM_Move_Hover(const cScript_FSM_Move_Hover& _other)
    : I_FSM(_other)
{
}

eFSM_RESULT cScript_FSM_Move_Hover::CheckCondition(const tFSM_Event& _tEvent)
{
    return eFSM_RESULT::REJECT;
}

void cScript_FSM_Move_Hover::EnterState(const tFSM_Event& _tEvent)
{
}

void cScript_FSM_Move_Hover::OnState()
{
}

void cScript_FSM_Move_Hover::EndState()
{
}
