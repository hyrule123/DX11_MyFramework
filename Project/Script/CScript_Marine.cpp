#include "pch.h"
#include "CScript_Marine.h"

#include "CScript_FSM_Idle.h"
#include "CScript_FSM_Move_Ground.h"
#include "CScript_FSM_AttackBegin.h"
#include "CScript_FSM_Attack.h"
#include "CScript_FSM_AttackEnd.h"

CScript_Marine::CScript_Marine(const string& _strKey)
	: CScript_SCEntity(_strKey)
{
	ScriptHolder()->AddFSM(new CScript_FSM_Idle);

	CScript_FSM_Move_Ground* move = new CScript_FSM_Move_Ground;
	move->SetSpeed(100.f);
	ScriptHolder()->AddFSM(FSM_SCGroundUnit::MOVE, move);

	ScriptHolder()->AddFSM(FSM_SCGroundUnit::ATTACK_BEGIN, new CScript_FSM_AttackBegin);
	ScriptHolder()->AddFSM(FSM_SCGroundUnit::ATTACK, new CScript_FSM_Attack);
	ScriptHolder()->AddFSM(FSM_SCGroundUnit::ATTACK_END, new CScript_FSM_AttackEnd);
}


CScript_Marine::~CScript_Marine()
{
}
