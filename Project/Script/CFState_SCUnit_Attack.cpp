#include "pch.h"
#include "CFState_SCUnit_Attack.h"

#include <Engine/CAnimator2D.h>

CFState_SCUnit_Attack::CFState_SCUnit_Attack()
	: CFState(TYPE_INDEX(CFState_SCUnit_Attack), (UINT)FSM_SCGroundUnit::eSTATE::ATTACK)
	, m_uDefaultDamage()
	, m_uDamageAddedPerUpgrade()
	, m_uWeaponRange()
{
}

CFState_SCUnit_Attack::~CFState_SCUnit_Attack()
{
}

void CFState_SCUnit_Attack::EnterState()
{
	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKeyAnim[eSTATE::ATTACK]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CFState_SCUnit_Attack::OnState()
{
}

void CFState_SCUnit_Attack::EndState()
{
}

bool CFState_SCUnit_Attack::CheckCondition(UINT _eState)
{
	if (FSM_MARINE::DEATH == _eState)
		return true;

	return false;
}
