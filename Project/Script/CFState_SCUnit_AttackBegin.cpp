#include "pch.h"
#include "CFState_SCUnit_AttackBegin.h"

#include "define_SCUnit.h"
#include <Engine/CAnimator2D.h>

#include <Engine/CFStateMgr.h>


CFState_SCUnit_AttackBegin::CFState_SCUnit_AttackBegin()
	: CFState(TYPE_INDEX(CFState_SCUnit_AttackBegin), FSM_MARINE::)
{
}

CFState_SCUnit_AttackBegin::~CFState_SCUnit_AttackBegin()
{
}

void CFState_SCUnit_AttackBegin::EnterState()
{
	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKeyAnim[eSTATE::ATTACK]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CFState_SCUnit_AttackBegin::OnState()
{
	if (Animator2D())
	{
		if (Animator2D()->IsFinished())
		{
			GetFStateMgr()->Transition(FSM_MARINE::ATTACK);
		}
	}
		

}

void CFState_SCUnit_AttackBegin::EndState()
{
}

bool CFState_SCUnit_AttackBegin::CheckCondition(UINT _eState)
{
	//사망 외엔 return false(전환 불가)
	if (FSM_MARINE::DEATH == _eState)
		return true;

	return false;
}
