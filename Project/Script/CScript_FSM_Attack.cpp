#include "pch.h"

#include "CScript_FSM_Attack.h"
#include "CScript_FSM_Move_Ground.h"

#include <Engine/CAnimator2D.h>


CScript_FSM_Attack::CScript_FSM_Attack(const string& _strKey)
	: CFSM(_strKey, (UINT)FSM_SCUnit::eSTATE::ATTACK)
	, m_uDefaultDMG()
	, m_uDMGAddedPerUpgrade()
	, m_uWeaponRange()
	, m_bStopReserved()
{
}

CScript_FSM_Attack::~CScript_FSM_Attack()
{
}

//공격 모션 시작
void CScript_FSM_Attack::EnterState()
{


	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCUnit;
		//공격 시작 모션 재생에 실패할 경우(시작 모션 애니메이션이 없을 경우) 바로 공격 모션으로 넘어감
		pAnimator->Play(FSM_SCUnit::strKey_Anim::ATTACK, eANIM_LOOPMODE::NORMAL_LOOP, false);
	}
}

void CScript_FSM_Attack::OnState()
{
	CAnimator2D* pAnim2D = Animator2D();
	

}

void CScript_FSM_Attack::EndState()
{
	m_pTarget = nullptr;
	m_bStopReserved = false;
	m_v2ReservedDest = Vec2::Zero;
}

bool CScript_FSM_Attack::CheckCondition(UINT _eState, tEvent _tEventMsg)
{
	//사망시 무조건 true
	if (FSM_SCUnit::DEATH == _eState)
		return true;

	//이동 명령이 들어오면 현재 이동 전환이 가능한 상태인지 확인 
	//이동이 불가능할 경우 이동 예약만 해놓는다.
	//이동 예약 상태는 OnState에서 처리
	//
	if (FSM_SCUnit::MOVE == _eState)
	{
		return false;
	}	

	return false;
}

void CScript_FSM_Attack::Attack(CGameObject* _pTarget)
{
	

	//TODO: ATTACK
}



