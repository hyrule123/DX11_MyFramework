#include "pch.h"

#include "CFState_SCUnit_Attack.h"
#include "CFState_SCUnit_AttackEnd.h"

#include <Engine/CAnimator2D.h>

#include <Engine/CFSM_Mgr.h>



CFState_SCUnit_Attack::CFState_SCUnit_Attack()
	: CFSM((UINT)FSM_SCGroundUnit::eSTATE::ATTACK)
	, m_uDefaultDamage()
	, m_uDamageAddedPerUpgrade()
	, m_uWeaponRange()
	, m_bReservedMove()
{
}

CFState_SCUnit_Attack::~CFState_SCUnit_Attack()
{
}

void CFState_SCUnit_Attack::EnterState()
{
	m_bReservedMove = -1;

	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKey_Anim[eSTATE::ATTACK]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CFState_SCUnit_Attack::OnState()
{
	if (Animator2D()->IsFinished() && 0 < m_bReservedMove)
	{
		CFState_SCUnit_AttackEnd* pAtkEnd = (CFState_SCUnit_AttackEnd*)GetFStateMgr()->Transition(FSM_SCGroundUnit::ATTACK_END);

	}
}

void CFState_SCUnit_Attack::EndState()
{
}

bool CFState_SCUnit_Attack::CheckCondition(UINT _eState)
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
