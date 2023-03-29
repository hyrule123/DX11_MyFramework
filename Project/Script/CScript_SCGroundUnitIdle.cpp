#include "pch.h"
#include "CScript_SCGroundUnitIdle.h"

#include <Engine/CAnimator2D.h>

#include "CScript_SCGroundUnitBase.h"

CScript_SCGroundUnitIdle::CScript_SCGroundUnitIdle()
    : CFSM(TYPE_INDEX(CScript_SCGroundUnitIdle), (UINT)SCGroundUnit::eSTATE::IDLE)
{
}

CScript_SCGroundUnitIdle::~CScript_SCGroundUnitIdle()
{
}


void CScript_SCGroundUnitIdle::init()
{
    CScript_SCGroundUnitBase* pMgr = static_cast<CScript_SCGroundUnitBase*>(GetHolder()->GetScript(TYPE_INDEX(CScript_SCGroundUnitBase)));

    if (nullptr == pMgr)
    {
        pMgr = new CScript_SCGroundUnitBase;
        GetHolder()->AddScript(pMgr);
    }
        
    pMgr->AddFSM(GetMyState(), this);
}

void CScript_SCGroundUnitIdle::EnterState()
{
    CAnimator2D* pAnimator = Animator2D();
    if (pAnimator)
    {
        using namespace SCGroundUnit;
        pAnimator->Play(string(strKeyAnim2D[(int)eSTATE::IDLE]), eANIM_LOOPMODE::NORMAL_LOOP, false);
    }
}

void CScript_SCGroundUnitIdle::OnState()
{
}

void CScript_SCGroundUnitIdle::EndState()
{
}

