#include "pch.h"
#include "CScript_FSM_Death.h"

#include "define_SCUnit.h"

#include <Engine/CAnimator2D.h>

#include <Engine/EventDispatcher.h>

CScript_FSM_Death::CScript_FSM_Death(const string& _strKey)
    :CFSM(_strKey, FSM_SCUnit::DEATH)
{
}


CScript_FSM_Death::~CScript_FSM_Death()
{
}

void CScript_FSM_Death::EnterState()
{
    CAnimator2D* pAnimator = Animator2D();
    assert(pAnimator);

    pAnimator->Play(FSM_SCUnit::strKey_Anim::DEATH, eANIM_LOOPMODE::NONE, false);

    //충돌체 제거
    EventDispatcher::RemoveComponent(GetOwner(), eCOMPONENT_TYPE::COLLIDER2D);
}

void CScript_FSM_Death::OnState()
{
    //사망 영상 재생 끝나면 일단 오브젝트 제거(나중에 잔해 오브젝트를 남겨놓던가 할것)
    if (Animator2D()->IsFinished())
    {
        EventDispatcher::DestroyGameObj(GetOwner());
    }
}

void CScript_FSM_Death::EndState()
{
}

