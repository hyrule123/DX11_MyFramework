#include "pch.h"
#include "CFState_SCGroundUnitIdle.h"

#include <Engine/CAnimator2D.h>

#include "CScript_SCGroundUnitFSM.h"

CFState_SCGroundUnitIdle::CFState_SCGroundUnitIdle()
    : CFState(TYPE_INDEX(CFState_SCGroundUnitIdle), (UINT)FSM_SCGroundUnit::eSTATE::IDLE)
{
}

CFState_SCGroundUnitIdle::~CFState_SCGroundUnitIdle()
{
}


void CFState_SCGroundUnitIdle::init()
{
}

void CFState_SCGroundUnitIdle::EnterState()
{
    CAnimator2D* pAnimator = Animator2D();
    if (pAnimator)
    {
        using namespace FSM_SCGroundUnit;
        pAnimator->Play(string(strKeyAnim2D[(int)eSTATE::IDLE]), eANIM_LOOPMODE::NORMAL_LOOP, false);
    }
}

void CFState_SCGroundUnitIdle::OnState()
{
}

void CFState_SCGroundUnitIdle::EndState()
{
}
