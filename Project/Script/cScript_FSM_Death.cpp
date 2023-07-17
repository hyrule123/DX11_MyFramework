#include "pch.h"
#include "cScript_FSM_Death.h"

#include "define_SC.h"

#include <Engine/cCom_Animator2D.h>

#include <Engine/EventDispatcher.h>

cScript_FSM_Death::cScript_FSM_Death(const string_view _strKey)
    :I_FSM(_strKey, SC::FSM::DEATH)
{
}


cScript_FSM_Death::~cScript_FSM_Death()
{
}

void cScript_FSM_Death::EnterState(const tFSM_Event& _tEvent)
{
    cCom_Animator2D* pAnimator = Animator2D();
    assert(pAnimator);

    pAnimator->Play(SC::FSM::strKey_Anim::DEATH, eANIM_PLAYMODE::NONE, false);

    //충돌체 제거
    EventDispatcher::RemoveComponent(GetOwner(), eCOMPONENT_TYPE::COLLIDER2D);
}

void cScript_FSM_Death::OnState()
{
    //사망 영상 재생 끝나면 일단 오브젝트 제거(나중에 잔해 오브젝트를 남겨놓던가 할것)
    if (Animator2D()->IsFinished())
    {
        EventDispatcher::DestroyGameObj(GetOwner());
    }
}

void cScript_FSM_Death::EndState()
{
}

