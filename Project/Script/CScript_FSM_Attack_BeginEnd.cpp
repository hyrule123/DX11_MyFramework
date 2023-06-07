#include "pch.h"
#include "CScript_FSM_Attack_BeginEnd.h"

#include <Engine/CAnimator2D.h>

CScript_FSM_Attack_BeginEnd::CScript_FSM_Attack_BeginEnd(const string& _strKey)
    : CFSM(_strKey, FSM_SCUnit::ATTACK_BEGIN_END)
    , m_bIsBegin()
    , m_uNextMoveID(FSM_SCUnit::END)
{

}

CScript_FSM_Attack_BeginEnd::~CScript_FSM_Attack_BeginEnd()
{
}

CScript_FSM_Attack_BeginEnd::CScript_FSM_Attack_BeginEnd(const CScript_FSM_Attack_BeginEnd& _other)
    : CFSM(_other)
{
}

void CScript_FSM_Attack_BeginEnd::EnterState()
{
    
}

void CScript_FSM_Attack_BeginEnd::OnState()
{
    //애니메이션 재생이 끝나고 다음 상태 전환 예약이 되어있을 경우
    if (FSM_SCUnit::END > m_uNextMoveID && Animator2D()->IsFinished())
    {
        //혹시나 전환 대상 state가 없을 경우에는 예약 초기화
        if (eFSM_RESULT::NULLPTR == Transition(m_uNextMoveID, m_tNextMoveMsg))
        {
            ResetNextMove();
        }
    }
}

void CScript_FSM_Attack_BeginEnd::EndState()
{
    ResetNextMove();
}

bool CScript_FSM_Attack_BeginEnd::CheckCondition(UINT _eStateID, tEvent _tEventMsg)
{
    m_bIsBegin = (bool)_tEventMsg.wParam;

    //사망은 무조건 허용
    if (FSM_SCUnit::DEATH == _eStateID)
        return true;

    CAnimator2D* pAnim = Animator2D();
    assert(pAnim);

    //공격 시작 및 준비동작 애니메이션은 캔슬 가능.
    //공격 종료 애니메이션은 캔슬할 수 없음. 대신 다음 행동 예약
    if (false == m_bIsBegin)
    {
        return true;
    }

    else if(Animator2D()->IsFinished())
    {
        return true;
    }

    //그 외의 경우에는 
    return false;
}

