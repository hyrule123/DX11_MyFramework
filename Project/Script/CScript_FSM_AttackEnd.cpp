#include "pch.h"
#include "CScript_FSM_AttackEnd.h"

#include "define_SCUnit.h"

#include <Engine/CAnimator2D.h>

CScript_FSM_AttackEnd::CScript_FSM_AttackEnd(const string& _strKey)
    : CFSM(_strKey, FSM_SCGroundUnit::ATTACK_END)
	, m_bCurMotionEnd()
{
}

CScript_FSM_AttackEnd::~CScript_FSM_AttackEnd()
{
}

void CScript_FSM_AttackEnd::EnterState()
{
	m_bCurMotionEnd = false;

	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKey_Anim[eSTATE::ATTACK]), eANIM_LOOPMODE::NONE, false);
	}
}

void CScript_FSM_AttackEnd::OnState()
{
	if (Animator2D()->IsFinished())
	{
		m_bCurMotionEnd = true;
		//GetFStateMgr(
	}
		
}

void CScript_FSM_AttackEnd::EndState()
{
}

bool CScript_FSM_AttackEnd::CheckCondition(UINT _eState, tEvent _tEventMsg)
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

