#include "pch.h"
#include "CFState_SCUnitAttack.h"

#include <Engine/CAnimator2D.h>

CFState_SCUnitAttack::CFState_SCUnitAttack()
	: CFState(TYPE_INDEX(CFState_SCUnitAttack), (UINT)FSM_SCGroundUnit::eSTATE::ATTACK)
{
}

CFState_SCUnitAttack::~CFState_SCUnitAttack()
{
}

void CFState_SCUnitAttack::EnterState()
{
	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKeyAnim2D[(UINT)eSTATE::ATTACK]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CFState_SCUnitAttack::OnState()
{
}

void CFState_SCUnitAttack::EndState()
{
}

bool CFState_SCUnitAttack::CheckCondition(UINT _eState)
{


	return true;
}
