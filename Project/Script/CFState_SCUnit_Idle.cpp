#include "pch.h"
#include "CFState_SCUnit_Idle.h"

#include <Engine/CAnimator2D.h>

#include "CSC_Entity.h"

CFState_SCUnit_Idle::CFState_SCUnit_Idle()
    : CFState((UINT)FSM_SCGroundUnit::eSTATE::IDLE)
{
}

CFState_SCUnit_Idle::~CFState_SCUnit_Idle()
{
}


void CFState_SCUnit_Idle::init()
{
}

void CFState_SCUnit_Idle::EnterState()
{
    CAnimator2D* pAnimator = Animator2D();
    if (pAnimator)
    {
        using namespace FSM_SCGroundUnit;
        pAnimator->Play(string(strKey_Anim[(int)eSTATE::IDLE]), eANIM_LOOPMODE::NORMAL_LOOP, false);
    }
}

void CFState_SCUnit_Idle::OnState()
{
}

void CFState_SCUnit_Idle::EndState()
{
}
