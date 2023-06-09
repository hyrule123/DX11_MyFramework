#include "pch.h"

#include "CScript_FSM_Attack.h"
#include "CScript_FSM_Attack_BeginEnd.h"
#include "CScript_FSM_Move_Ground.h"


#include <Engine/CAnimator2D.h>


CScript_FSM_Attack::CScript_FSM_Attack(const string& _strKey)
	: CFSM(_strKey, (UINT)FSM_SCUnit::eSTATE::ATTACK)
	, m_uDefaultDMG()
	, m_uDMGAddedPerUpgrade()
	, m_uWeaponRange()
	, m_pTarget()
{
}

CScript_FSM_Attack::~CScript_FSM_Attack()
{
}

void CScript_FSM_Attack::init()
{
	m_pAtkBeginEnd = static_cast<CScript_FSM_Attack_BeginEnd*>(ScriptHolder()->GetFSM(FSM_SCUnit::ATTACK_BEGIN_END));
	assert(nullptr != m_pAtkBeginEnd);
}

//공격 모션 시작
void CScript_FSM_Attack::EnterState(const tFSM_Event& _tEvent)
{
	//TODO: Target nullptr check

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
	//애니메이션 재생이 끝났을 경우 타겟이 있는지 다시 확인 후 지속
	CAnimator2D* pAnim2D = Animator2D();
	if (pAnim2D && pAnim2D->IsFinished())
	{

	}
}

void CScript_FSM_Attack::EndState()
{
	m_pTarget = nullptr;
}

eFSM_RESULT CScript_FSM_Attack::CheckCondition(const tFSM_Event& _tEvent)
{
	//사망시 무조건 true
	if (FSM_SCUnit::DEATH == _tEvent.uStateID)
		return eFSM_RESULT::ACCEPT;

	//이동 명령이 들어오면 현재 이동 전환이 가능한 상태인지 확인 
	//이동이 불가능할 경우 이동 예약만 해놓는다.
	//이동 예약 상태는 OnState에서 처리
	else if (FSM_SCUnit::MOVE == _tEvent.uStateID)
	{
		//아직 이동할 수 있는 상황이 아니면 이동 예약
		if(false == Animator2D()->IsFinished())
			return eFSM_RESULT::RESERVE;

		//이동할 수 있는 상황일 경우 강제 상태 변경
		ScriptHolder()->ForceTransition(FSM_SCUnit::ATTACK_BEGIN_END);
		return eFSM_RESULT::REJECT;		
	}	

	

	return eFSM_RESULT::REJECT;
}

void CScript_FSM_Attack::Attack(CGameObject* _pTarget)
{
	

	//TODO: ATTACK
}



