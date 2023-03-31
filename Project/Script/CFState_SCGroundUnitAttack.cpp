#include "pch.h"
#include "CFState_SCGroundUnitAttack.h"

CFState_SCGroundUnitAttack::CFState_SCGroundUnitAttack()
	: CFState(TYPE_INDEX(CFState_SCGroundUnitAttack), (UINT)FSM_SCGroundUnit::eSTATE::ATTACK)
{
}

CFState_SCGroundUnitAttack::~CFState_SCGroundUnitAttack()
{
}

void CFState_SCGroundUnitAttack::EnterState()
{
}

void CFState_SCGroundUnitAttack::OnState()
{
}

void CFState_SCGroundUnitAttack::EndState()
{
}

bool CFState_SCGroundUnitAttack::CheckCondition(UINT _eState)
{


	return true;
}
