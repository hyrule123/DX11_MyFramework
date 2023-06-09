#include "pch.h"
#include "CScript_FSM_Idle.h"

#include <Engine/CAnimator2D.h>

#include "CScript_SCEntity.h"

CScript_FSM_Idle::CScript_FSM_Idle(const string& _strKey)
    : CFSM(_strKey, (UINT)FSM_SCUnit::eSTATE::IDLE)
{
}

CScript_FSM_Idle::~CScript_FSM_Idle()
{
}


bool CScript_FSM_Idle::SaveJson(Json::Value* _pJson)
{
    try
    {
        if (nullptr == _pJson)
            throw(std::runtime_error("_pJson == nullptr"));

        else if (false == CFSM::SaveJson(_pJson))
            return false;

        Json::Value& jVal = *_pJson;




    }
    catch (const std::runtime_error& err)
    {
        ERROR_MESSAGE(err.what());
        assert(false);
        return false;
    }

    return true;
}

bool CScript_FSM_Idle::LoadJson(Json::Value* _pJson)
{
    try
    {
        if (nullptr == _pJson)
            throw(std::runtime_error("_pJson == nullptr"));

        else if (false == CFSM::LoadJson(_pJson))
            return false;

        Json::Value& jVal = *_pJson;


    }
    catch (const std::runtime_error& err)
    {
        ERROR_MESSAGE(err.what());
        assert(false);
        return false;
    }

    return true;

}

void CScript_FSM_Idle::init()
{
}

void CScript_FSM_Idle::EnterState(const tFSM_Event& _tEvent)
{
    CAnimator2D* pAnimator = Animator2D();
    if (pAnimator)
    {
        using namespace FSM_SCUnit;
        pAnimator->Play(strKey_Anim::IDLE, eANIM_LOOPMODE::NORMAL_LOOP, false);
    }
}

void CScript_FSM_Idle::OnState()
{
}

void CScript_FSM_Idle::EndState()
{
}

