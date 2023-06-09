#include "pch.h"

#include "CScript_FSM_Attack.h"
#include "CScript_FSM_Move_Ground.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CTimeMgr.h>


CScript_FSM_Attack::CScript_FSM_Attack(const string& _strKey)
	: CFSM(_strKey, (UINT)FSM_SCUnit::eSTATE::ATTACK)
	, m_uDefaultDMG()
	, m_uDMGAddedPerUpgrade()
	, m_fAtkInterval(0.5f)
	, m_uWeaponRange()
	, m_pTarget()
{
}

CScript_FSM_Attack::~CScript_FSM_Attack()
{
}

//공격 모션 시작
void CScript_FSM_Attack::EnterState(const tFSM_Event& _tEvent)
{
	//TODO: Target nullptr check
	m_pTarget = reinterpret_cast<CGameObject*>(_tEvent.destParam);
	

	m_eAtkState = eATTACK_STATE::BEGIN_ATTACK;
	CAnimator2D* pAnim = Animator2D();
	assert(nullptr != pAnim);

	//공격 시작 애니메이션 재생, 실패(애니메이션 없음) 시 즉시 공격
	if (false == pAnim->Play(FSM_SCUnit::strKey_Anim::ATTACK_BEGIN_END, eANIM_LOOPMODE::NONE, false))
	{
		m_eAtkState = eATTACK_STATE::ATTACKING;
		pAnim->Play(FSM_SCUnit::strKey_Anim::ATTACK, eANIM_LOOPMODE::NONE, false);
	}

}

void CScript_FSM_Attack::OnState()
{
	//애니메이션 재생이 끝났을 경우 타겟이 있는지 다시 확인 후 지속
	CAnimator2D* pAnim = Animator2D();
	assert(nullptr != pAnim);
	if (pAnim->IsFinished())
	{
		switch (m_eAtkState)
		{
		case eATTACK_STATE::BEGIN_ATTACK:
			m_eAtkState = eATTACK_STATE::ATTACKING;
			pAnim->Play(FSM_SCUnit::strKey_Anim::ATTACK, eANIM_LOOPMODE::NONE, false);
			break;
		case eATTACK_STATE::ATTACKING:
			m_fCurInterval -= DELTA_TIME;
			if (m_fCurInterval < 0.f)
			{
				m_fCurInterval = m_fAtkInterval;

				//다시 공격 시작

			}
			break;
		case eATTACK_STATE::END_ATTACK:
			break;
		default:
			break;
		}
		
		//공격대상이 있는지 확인 후 다시 시작
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

void CScript_FSM_Attack::Attack()
{

}




