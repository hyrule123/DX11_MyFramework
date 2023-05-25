#include "pch.h"

#include "CScript_FSM_Attack.h"
#include "CScript_FSM_AttackEnd.h"

#include <Engine/CAnimator2D.h>

CScript_FSM_Attack::CScript_FSM_Attack(const string& _strKey)
	: CFSM(_strKey, (UINT)FSM_SCGroundUnit::eSTATE::ATTACK)
	, m_uDefaultDamage()
	, m_uDamageAddedPerUpgrade()
	, m_uWeaponRange()
	, m_bReservedMove()
{
}

CScript_FSM_Attack::~CScript_FSM_Attack()
{
}

void CScript_FSM_Attack::EnterState()
{
	m_bReservedMove = -1;

	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKey_Anim[eSTATE::ATTACK]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CScript_FSM_Attack::OnState()
{
	if (Animator2D()->IsFinished() && 0 < m_bReservedMove)
	{
		CScript_FSM_AttackEnd* pAtkEnd = (CScript_FSM_AttackEnd*)ScriptHolder()->Transition(FSM_SCGroundUnit::ATTACK_END);

	}
}

void CScript_FSM_Attack::EndState()
{
}

bool CScript_FSM_Attack::CheckCondition(UINT _eState, tEvent _tEventMsg)
{
	if (FSM_SCGroundUnit::DEATH == _eState)
		return true;

	if (FSM_SCGroundUnit::MOVE == _eState)
	{
		m_bReservedMove = (UINT)FSM_SCGroundUnit::MOVE;
	}

	if (FSM_SCGroundUnit::ATTACK_END == _eState)
	{
		if (m_bReservedMove)
			return true;
	}
	

	return false;
}
