#include "pch.h"
#include "CScript_Marine.h"

#include "CFState_SCUnitIdle.h"
#include "CFState_SCUnitMove_Ground.h"
#include "CFState_SCUnitAttack.h"

CScript_Marine::CScript_Marine()
	: CSCEntity(TYPE_INDEX(CScript_Marine), (UINT)FSM_SCGroundUnit::eSTATE::END)
{
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::IDLE, new CFState_SCUnitIdle);
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::MOVE, new CFState_SCUnitMove_Ground);
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::ATTACK, new CFState_SCUnitAttack);
}


CScript_Marine::~CScript_Marine()
{
}
