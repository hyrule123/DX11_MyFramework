#include "pch.h"
#include "CFState_SCUnit_AttackEnd.h"

#include "define_SCUnit.h"

#include <Engine/CAnimator2D.h>

CFState_SCUnit_AttackEnd::CFState_SCUnit_AttackEnd()
    : CFState(FSM_SCGroundUnit::ATTACK_END)
	, m_bCurMotionEnd()
{
}

CFState_SCUnit_AttackEnd::~CFState_SCUnit_AttackEnd()
{
}

void CFState_SCUnit_AttackEnd::EnterState()
{
	m_bCurMotionEnd = false;

	CAnimator2D* pAnimator = GetFStateMgr()->Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKey_Anim[eSTATE::ATTACK]), eANIM_LOOPMODE::NONE, false);
	}
}

void CFState_SCUnit_AttackEnd::OnState()
{
	if (Animator2D()->IsFinished())
	{
		m_bCurMotionEnd = true;
		//GetFStateMgr(
	}
		
}

void CFState_SCUnit_AttackEnd::EndState()
{
}

bool CFState_SCUnit_AttackEnd::CheckCondition(UINT _eState)
{
	using namespace FSM_SCGroundUnit;
	switch (_eState)
	{
	case IDLE:
		return false;
		
	case MOVE:
		if (m_bCurMotionEnd)
			return true;
		return false;
		
	case ATTACK_BEGIN:
		return false;

	case ATTACK:
		return false;
		
	case ATTACK_END:
		return false;
		
	case DEATH:
		return true;
		
	default:
		break;
	}

    return false;
}

