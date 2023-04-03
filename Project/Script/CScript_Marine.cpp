#include "pch.h"
#include "CScript_Marine.h"

#include "CFState_SCUnitIdle.h"
#include "CFState_SCUnitMove_Ground.h"
#include "CFState_SCUnitAttack.h"

CScript_Marine::CScript_Marine()
	: CSCEntity(TYPE_INDEX(CScript_Marine), (UINT)FSM_SCGroundUnit::eSTATE::END)
{
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::IDLE, new CFState_SCUnitIdle);
	CFState_SCUnitMove_Ground* move = new CFState_SCUnitMove_Ground;
	move->SetSpeed(100.f);
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::MOVE, move);
	AddFState((UINT)FSM_SCGroundUnit::eSTATE::ATTACK, new CFState_SCUnitAttack);
}


CScript_Marine::~CScript_Marine()
{
}
