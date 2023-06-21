#include "pch.h"
#include "CScript_VespeneSmoke.h"

#include "define_SC.h"
#include <Engine/CAnimator2D.h>

#include <Engine/CTransform.h>

//랜덤 딜레이
#include <Engine/CRandMgr.h>
#include <Engine/CTimeMgr.h>

#define SMOKE_DELAY_MIN 0.5f
#define SMOKE_DELAY_MAX 1.f

CScript_VespeneSmoke::CScript_VespeneSmoke(const string& _strKey)
	: CScript_SCEntity(_strKey)
	, m_fRandVal()
{
}

CScript_VespeneSmoke::~CScript_VespeneSmoke()
{
}

void CScript_VespeneSmoke::init()
{
	CScript_SCEntity::init();

	Animator2D()->PreparePlay(SC::strKey_Anim::Neutral::VESPENE_SMOKE_1, eANIM_PLAYMODE::DISABLE_AFTER_PLAY, false);

	m_fRandVal = CRandMgr::GetInst()->GetRand<float>(SMOKE_DELAY_MIN, SMOKE_DELAY_MAX);
}	

void CScript_VespeneSmoke::tick()
{
	//애니메이션이 종료되었을 경우 랜덤한 값으로 딜레이를 준다. 
	if (Animator2D()->IsFinished())
	{
		if (0.f > m_fRandVal)
		{	
			Animator2D()->PlayAgain();
			m_fRandVal = CRandMgr::GetInst()->GetRand<float>(SMOKE_DELAY_MIN, SMOKE_DELAY_MAX);
		}
		else
		{
			m_fRandVal -= DELTA_TIME;
		}
	}
}
