#include "pch.h"
#include "CFState_SCUnit_Death.h"

#include "define_SCUnit.h"

CFState_SCUnit_Death::CFState_SCUnit_Death()
    :CFState(FSM_SCGroundUnit::DEATH)
{
}


CFState_SCUnit_Death::~CFState_SCUnit_Death()
{
}

void CFState_SCUnit_Death::EnterState()
{
}

void CFState_SCUnit_Death::OnState()
{
}

void CFState_SCUnit_Death::EndState()
{
}

bool CFState_SCUnit_Death::CheckCondition(UINT _eState)
{
    return false;
}
