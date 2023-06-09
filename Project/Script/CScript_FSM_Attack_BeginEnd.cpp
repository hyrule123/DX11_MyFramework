#include "pch.h"
#include "CScript_FSM_Attack_BeginEnd.h"

#include <Engine/CAnimator2D.h>


CScript_FSM_Attack_BeginEnd::CScript_FSM_Attack_BeginEnd(const string& _strKey)
    : CFSM(_strKey, FSM_SCUnit::ATTACK_BEGIN_END)
    , m_bIsEnd()
{

}

CScript_FSM_Attack_BeginEnd::~CScript_FSM_Attack_BeginEnd()
{
}

CScript_FSM_Attack_BeginEnd::CScript_FSM_Attack_BeginEnd(const CScript_FSM_Attack_BeginEnd& _other)
    : CFSM(_other)
    , m_bIsEnd(_other.m_bIsEnd)
{
}

void CScript_FSM_Attack_BeginEnd::EnterState(const tFSM_Event& _tEvent)
{
    m_bIsEnd = (bool)_tEvent.destParam;

    //애니메이션이 없을 경우 바로 공격 애니메이션 또는 IDLE 애니메이션으로 전환
    using namespace FSM_SCUnit;
    if (false == Animator2D()->Play(strKey_Anim::ATTACK_BEGIN_END, eANIM_LOOPMODE::NONE, m_bIsEnd))
    {
        if (false == m_bIsEnd)
            Transition(FSM_SCUnit::ATTACK);
        else
            Transition(FSM_SCUnit::IDLE);
    }
}

void CScript_FSM_Attack_BeginEnd::OnState()
{
    //재생이 끝나면 현재 동작에 따라 애니메이션 전환
    if (Animator2D()->IsFinished())
    {
        if (false == m_bIsEnd)
            Transition(FSM_SCUnit::ATTACK);
        else
            Transition(FSM_SCUnit::IDLE);
    }
}

void CScript_FSM_Attack_BeginEnd::EndState()
{
}

eFSM_RESULT CScript_FSM_Attack_BeginEnd::CheckCondition(const tFSM_Event& _tEvent)
{
    //사망으로의 전환은 무조건 허용
    if (FSM_SCUnit::DEATH == _tEvent.uStateID)
        return eFSM_RESULT::ACCEPT;

    //공격 시작 및 준비동작 애니메이션은 캔슬 가능.
    //공격 종료 애니메이션은 캔슬할 수 없음. 대신 다음 행동 예약
    if (false == m_bIsEnd)
    {
        return eFSM_RESULT::ACCEPT;
    }

    //공격 종료 애니메이션일 때 + 애니메이션 재생이 끝났을 때
    else if (Animator2D()->IsFinished())
    {
        return eFSM_RESULT::ACCEPT;
    }

    //공격 종료 애니메이션이 아직 재생 중일때 - 행동 예약
    else
    {
        return eFSM_RESULT::RESERVE;
    }


    //그 외의 경우에는 
    return eFSM_RESULT::REJECT;
}

