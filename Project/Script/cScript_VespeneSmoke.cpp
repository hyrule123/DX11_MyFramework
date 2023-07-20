#include "pch.h"
#include "cScript_VespeneSmoke.h"

#include <Engine/cGameObject.h>
#include <Engine/cCom_Animator2D.h>
#include <Engine/cTransform.h>
#include <Engine/RandGen.h>
#include <Engine/cTimeMgr.h>

#include "define_SC.h"




#define SMOKE_DELAY_MIN 0.5f
#define SMOKE_DELAY_MAX 1.f

cScript_VespeneSmoke::cScript_VespeneSmoke()
	: m_fRandVal()
	, m_pAnimator()
{
}

cScript_VespeneSmoke::~cScript_VespeneSmoke()
{
}

void cScript_VespeneSmoke::Init()
{
	cScript_SCEntity::Init();

	m_pAnimator = GetOwner()->GetComponent<cCom_Animator2D>();
	
	m_pAnimator->PreparePlay(SC::strKey_Anim::Neutral::VESPENE_SMOKE_1, eANIM_PLAYMODE::DISABLE_AFTER_PLAY, false);

	m_fRandVal = RandGen::GetRand<float>(SMOKE_DELAY_MIN, SMOKE_DELAY_MAX);
}	

void cScript_VespeneSmoke::Tick()
{
	//애니메이션이 종료되었을 경우 랜덤한 값으로 딜레이를 준다. 
	if (m_pAnimator->IsFinished())
	{
		if (0.f > m_fRandVal)
		{	
			m_pAnimator->PlayAgain();
			m_fRandVal = RandGen::GetRand<float>(SMOKE_DELAY_MIN, SMOKE_DELAY_MAX);
		}
		else
		{
			m_fRandVal -= DELTA_TIME;
		}
	}
}
