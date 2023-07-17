#include "pch.h"
#include "cScript_FSM_Idle.h"

#include <Engine/cCom_Animator2D.h>

#include "cScript_SCEntity.h"

cScript_FSM_Idle::cScript_FSM_Idle(const string_view _strKey)
    : I_FSM(_strKey, (UINT)SC::FSM::IDLE)
{
}

cScript_FSM_Idle::~cScript_FSM_Idle()
{
}


bool cScript_FSM_Idle::SaveJson(Json::Value* _pJson)
{
    try
    {
        if (nullptr == _pJson)
            throw(std::runtime_error("_pJson == nullptr"));

        else if (false == I_FSM::SaveJson(_pJson))
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

bool cScript_FSM_Idle::LoadJson(Json::Value* _pJson)
{
    try
    {
        if (nullptr == _pJson)
            throw(std::runtime_error("_pJson == nullptr"));

        else if (false == I_FSM::LoadJson(_pJson))
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

void cScript_FSM_Idle::init()
{
}

void cScript_FSM_Idle::EnterState(const tFSM_Event& _tEvent)
{
    cCom_Animator2D* pAnimator = Animator2D();
    if (pAnimator)
    {
        using namespace SC;
        pAnimator->Play(FSM::strKey_Anim::IDLE, eANIM_PLAYMODE::NORMAL_LOOP, false);
    }
}

void cScript_FSM_Idle::OnState()
{
}

void cScript_FSM_Idle::EndState()
{
}

