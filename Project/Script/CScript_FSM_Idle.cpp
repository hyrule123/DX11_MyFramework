#include "pch.h"
#include "CScript_FSM_Idle.h"

#include <Engine/CAnimator2D.h>

#include "CScript_SCEntity.h"

CScript_FSM_Idle::CScript_FSM_Idle(const string& _strKey)
    : CFSM(_strKey, (UINT)FSM_SCGroundUnit::eSTATE::IDLE)
{
}

CScript_FSM_Idle::~CScript_FSM_Idle()
{
}


void CScript_FSM_Idle::init()
{
}

void CScript_FSM_Idle::EnterState()
{
    CAnimator2D* pAnimator = Animator2D();
    if (pAnimator)
    {
        using namespace FSM_SCGroundUnit;
        pAnimator->Play(string(strKey_Anim[(int)eSTATE::IDLE]), eANIM_LOOPMODE::NORMAL_LOOP, false);
    }
}

void CScript_FSM_Idle::OnState()
{
}

void CScript_FSM_Idle::EndState()
{
}
