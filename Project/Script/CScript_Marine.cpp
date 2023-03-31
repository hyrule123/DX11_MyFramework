#include "pch.h"
#include "CScript_Marine.h"

#include "CFState_SCGroundUnitIdle.h"
#include "CFState_SCGroundUnitMove.h"
#include "CFState_SCGroundUnitAttack.h"

CScript_Marine::CScript_Marine()
{
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::IDLE, new CFState_SCGroundUnitIdle);
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::MOVE, new CFState_SCGroundUnitMove);
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::ATTACK, new CFState_SCGroundUnitAttack);
}


CScript_Marine::~CScript_Marine()
{
}
