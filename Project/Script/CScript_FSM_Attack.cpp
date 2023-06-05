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
	, m_eAttackState()
{
}

CScript_FSM_Attack::~CScript_FSM_Attack()
{
}

//공격 모션 시작
void CScript_FSM_Attack::EnterState()
{

	m_eAttackState = eSCUNIT_ATTACK_STATE::BEGIN_ATTACK;

	CAnimator2D* pAnimator = Animator2D();
	if (pAnimator)
	{
		using namespace FSM_SCUnit;
		//공격 시작 모션 재생에 실패할 경우(시작 모션 애니메이션이 없을 경우) 바로 공격 모션으로 넘어감
		if (false == pAnimator->Play(strKey_Anim::ATTACK_BEGIN, eANIM_LOOPMODE::NONE, false))
		{
			m_eAttackState = eSCUNIT_ATTACK_STATE::IN_ATTACK;
			pAnimator->Play(strKey_Anim::ATTACK, eANIM_LOOPMODE::NORMAL_LOOP, false);
		}
	}
}

void CScript_FSM_Attack::OnState()
{
	CAnimator2D* pAnim2D = Animator2D();
	

	//TODO:타겟 처리 코드 추가

	switch (m_eAttackState)
	{
	case eSCUNIT_ATTACK_STATE::IDLE:
	case eSCUNIT_ATTACK_STATE::BEGIN_ATTACK:
	{
		//이동 예약이 되었는지부터 체크
		if (m_bStopReserved)
		{
			m_bStopReserved = false;
			Move(m_v2ReservedDest);
		}

		//공격 시작 모션 재생이 끝났을 경우 상태 변경하고 공격 애니메이션 재생 시작
		else if (pAnim2D->IsFinished())
		{
			using namespace FSM_SCUnit;
			m_eAttackState = eSCUNIT_ATTACK_STATE::IN_ATTACK;
			pAnim2D->Play(strKey_Anim::ATTACK, eANIM_LOOPMODE::NORMAL_LOOP, false);
		}

		break;
	}

	case eSCUNIT_ATTACK_STATE::IN_ATTACK:
	{
		using namespace FSM_SCUnit;

		//더이상 타겟이 존재하지 않거나
		//이동 명령이 예약되어 있고, 공격 모션이 끝났을 경우 공격 모션을 중단
		if (nullptr == m_pTarget
			||
			(m_bStopReserved && pAnim2D->IsFinished()))
		{
			m_bStopReserved = false;

			//종료 애니메이션은 준비동작의 역재생
			//재생에 실패했을 경우(공격 준비동작 애니메이션은 없을경우) 바로 State 전환
			if (false == pAnim2D->Play(FSM_SCUnit::strKey_Anim::ATTACK_BEGIN_END, eANIM_LOOPMODE::NONE, true))
			{
				Transition
			}
			

			m_eAttackState = eSCUNIT_ATTACK_STATE::END_ATTACK;
		}

		//정지 명령이 들어오지 않은 경우 타겟을 향해 계속 공격(나중에 투사체 오브젝트 생성 코드 추가해야함)
		else
		{

		}

		break;
	}
		
	case eSCUNIT_ATTACK_STATE::END_ATTACK:
	{
		if(pAnim2D->IsFinished())


		break;
	}
		
	default:
		break;
	}


		//switch (eSCUNIT_ATTACK_STATE)
	//{
	//case eSCUNIT_ATTACK_STATE::IDLE:
	//	break;
	//case eSCUNIT_ATTACK_STATE::BEGIN_ATTACK:
	//	break;
	//case eSCUNIT_ATTACK_STATE::IN_ATTACK:
	//	break;
	//case eSCUNIT_ATTACK_STATE::END_ATTACK:
	//	break;
	//default:
	//	break;
	//}
}

void CScript_FSM_Attack::EndState()
{
	m_eAttackState = eSCUNIT_ATTACK_STATE::IDLE;
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
		switch (m_eAttackState)
		{
		case eSCUNIT_ATTACK_STATE::IDLE:
		case eSCUNIT_ATTACK_STATE::BEGIN_ATTACK:
			//공격 시작 모션 도중일 경우에는 즉시 전환 가능
			//여기에 들어온다는 것은 Move FSM이 있다는 뜻임
			CFSM_Move_Base* pMoveScript =
				static_cast<CFSM_Move_Base*>(ScriptHolder()->GetFSM(FSM_SCUnit::MOVE));
			if (pMoveScript)
				pMoveScript->SetDestination(Pack_v2_i64(_tEventMsg.wParam).v2);
			return true;

		//공격 중, 공격 종료 모션 중에는 즉시 전환 불가능.
		case eSCUNIT_ATTACK_STATE::IN_ATTACK:
		case eSCUNIT_ATTACK_STATE::END_ATTACK:
			m_bStopReserved = true;
			m_v2ReservedDest = Pack_v2_i64(_tEventMsg.wParam).v2;
			return false;

		default:
			break;
		}


		return false;
	}	

	return false;
}

void CScript_FSM_Attack::Attack(CGameObject* _pTarget)
{
	

	//TODO: ATTACK
}



