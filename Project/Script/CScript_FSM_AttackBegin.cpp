#include "pch.h"
#include "CScript_FSM_AttackBegin.h"

#include "define_SCUnit.h"
#include <Engine/CAnimator2D.h>


CScript_FSM_AttackBegin::CScript_FSM_AttackBegin(const string& _strKey)
	: CFSM(_strKey, FSM_SCGroundUnit::ATTACK_BEGIN)
{
}

CScript_FSM_AttackBegin::~CScript_FSM_AttackBegin()
{
}

void CScript_FSM_AttackBegin::EnterState()
{
	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCGroundUnit;
		pAnimator->Play(string(strKey_Anim[eSTATE::ATTACK]), eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CScript_FSM_AttackBegin::OnState()
{
	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		if (pAnimator->IsFinished())
		{
			ScriptHolder()->Transition(FSM_SCGroundUnit::ATTACK);
		}
	}
		

}

void CScript_FSM_AttackBegin::EndState()
{
}

bool CScript_FSM_AttackBegin::CheckCondition(UINT _eState, tEvent _tEventMsg)
{
	//사망 외엔 return false(전환 불가)
	if (FSM_SCGroundUnit::DEATH == _eState)
		return true;

	return false;
}
