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
	, m_eAtkState()
	, m_bIsReadyAnim()
{
	m_fCurInterval = m_fAtkInterval;
}

CScript_FSM_Attack::~CScript_FSM_Attack()
{
}

void CScript_FSM_Attack::init()
{
	//공격 준비 모션 애니메이션이 있는지 여부를 확인한다.
	if (Animator2D()->FindAnim(FSM_SCUnit::strKey_Anim::ATTACK_BEGIN_END))
		m_bIsReadyAnim = true;

	//Animator 유무도 확인
	assert(Animator2D());
}

//공격 모션 시작
void CScript_FSM_Attack::EnterState(const tFSM_Event& _tEvent)
{
	//TODO: Target nullptr check
	m_pTarget = reinterpret_cast<CGameObject*>(_tEvent.destParam);

	//준비모션 애니메이션이 없을 경우 BEGIN_ATTACK과 END_ATTACK 상태는 아예 들어오지 않음
	if (m_bIsReadyAnim)
	{
		m_eAtkState = eATTACK_STATE::BEGIN_ATTACK;
		Animator2D()->Play(FSM_SCUnit::strKey_Anim::ATTACK_BEGIN_END, eANIM_LOOPMODE::NONE, false);
	}
	else
	{
		m_eAtkState = eATTACK_STATE::ATTACKING;
		Animator2D()->Play(FSM_SCUnit::strKey_Anim::ATTACK, eANIM_LOOPMODE::NONE, false);
	}
}

void CScript_FSM_Attack::OnState()
{
	//애니메이션 재생이 끝났을 경우 타겟이 있는지 다시 확인 후 지속
	CAnimator2D* pAnim = Animator2D();

	//애니메이션의 재생이 끝났을 경우 State에 따라서 다른 처리를 해준다.
	if (pAnim->IsFinished())
	{
		switch (m_eAtkState)
		{
		case eATTACK_STATE::BEGIN_ATTACK:
			m_eAtkState = eATTACK_STATE::ATTACKING;
			pAnim->Play(FSM_SCUnit::strKey_Anim::ATTACK, eANIM_LOOPMODE::NONE, false);
			break;
		case eATTACK_STATE::ATTACKING:
			//TODO: 타겟 유무 검사(죽었는지 안 죽었는지)

			m_fCurInterval -= DELTA_TIME;
			if (m_fCurInterval < 0.f)
			{
				//공격 인터벌 초기화
				m_fCurInterval = m_fAtkInterval;

				//다시 공격 시작
				Animator2D()->PlayAgain();
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
	else if (FSM_SCUnit::ATTACK == _tEvent.uStateID)
		return eFSM_RESULT::REJECT;

	switch (m_eAtkState)
	{
	case eATTACK_STATE::BEGIN_ATTACK:
		m_eAtkState = eATTACK_STATE::END_ATTACK;
		Animator2D()->ReverseCurrent();
		break;


	case eATTACK_STATE::ATTACKING:
		//공격 애니메이션이 끝났을 경우 상태 전환. 계속 Reserve 걸어놓음
		if (Animator2D()->IsFinished())
		{
			if (m_bIsReadyAnim)
			{
				m_eAtkState = eATTACK_STATE::END_ATTACK;
				Animator2D()->Play(FSM_SCUnit::strKey_Anim::ATTACK_BEGIN_END, eANIM_LOOPMODE::NONE, true);
				return eFSM_RESULT::RESERVE;
			}
			else
				return eFSM_RESULT::ACCEPT;
		}
		else
			return eFSM_RESULT::RESERVE;

		
	case eATTACK_STATE::END_ATTACK:
		if (Animator2D()->IsFinished())
		{
			return eFSM_RESULT::ACCEPT;
		}
		else
			return eFSM_RESULT::RESERVE;

		break;
	default:
		break;
	}


	return eFSM_RESULT::REJECT;
}




