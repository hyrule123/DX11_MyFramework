#include "pch.h"
#include "cScript_VespeneSmoke.h"

#include "define_SC.h"
#include <Engine/cCom_Animator2D.h>

#include <Engine/cCom_Transform.h>

//랜덤 딜레이
#include <Engine/RandGen.h>
#include <Engine/cTimeMgr.h>

#define SMOKE_DELAY_MIN 0.5f
#define SMOKE_DELAY_MAX 1.f

cScript_VespeneSmoke::cScript_VespeneSmoke(const string_view _strKey)
	: cScript_SCEntity(_strKey)
	, m_fRandVal()
{
}

cScript_VespeneSmoke::~cScript_VespeneSmoke()
{
}

void cScript_VespeneSmoke::init()
{
	cScript_SCEntity::init();

	Animator2D()->PreparePlay(SC::strKey_Anim::Neutral::VESPENE_SMOKE_1, eANIM_PLAYMODE::DISABLE_AFTER_PLAY, false);

	m_fRandVal = RandGen::GetInst()->GetRand<float>(SMOKE_DELAY_MIN, SMOKE_DELAY_MAX);
}	

void cScript_VespeneSmoke::tick()
{
	//애니메이션이 종료되었을 경우 랜덤한 값으로 딜레이를 준다. 
	if (Animator2D()->IsFinished())
	{
		if (0.f > m_fRandVal)
		{	
			Animator2D()->PlayAgain();
			m_fRandVal = RandGen::GetInst()->GetRand<float>(SMOKE_DELAY_MIN, SMOKE_DELAY_MAX);
		}
		else
		{
			m_fRandVal -= DELTA_TIME;
		}
	}
}
