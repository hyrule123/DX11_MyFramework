#include "pch.h"
#include "CFState_SCUnitIdle.h"

#include <Engine/CAnimator2D.h>

#include "CSCEntity.h"

CFState_SCUnitIdle::CFState_SCUnitIdle()
    : CFState((UINT)FSM_SCGroundUnit::eSTATE::IDLE)
{
}

CFState_SCUnitIdle::~CFState_SCUnitIdle()
{
}


void CFState_SCUnitIdle::init()
{
}

void CFState_SCUnitIdle::EnterState()
{
    CAnimator2D* pAnimator = Animator2D();
    if (pAnimator)
    {
        using namespace FSM_SCGroundUnit;
        pAnimator->Play(string(strKey_Anim[(int)eSTATE::IDLE]), eANIM_LOOPMODE::NORMAL_LOOP, false);
    }
}

void CFState_SCUnitIdle::OnState()
{
}

void CFState_SCUnitIdle::EndState()
{
}
