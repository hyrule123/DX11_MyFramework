#include "pch.h"
#include "CFState_SCGroundUnitAttack.h"

#include <Engine/CAnimator2D.h>

CFState_SCGroundUnitAttack::CFState_SCGroundUnitAttack()
	: CFState(TYPE_INDEX(CFState_SCGroundUnitAttack), (UINT)FSM_SCGroundUnit::eSTATE::ATTACK)
{
}

CFState_SCGroundUnitAttack::~CFState_SCGroundUnitAttack()
{
}

void CFState_SCGroundUnitAttack::EnterState()
{
	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKeyAnim2D[(UINT)eSTATE::ATTACK]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CFState_SCGroundUnitAttack::OnState()
{
}

void CFState_SCGroundUnitAttack::EndState()
{
}

bool CFState_SCGroundUnitAttack::CheckCondition(UINT _eState)
{


	return true;
}
