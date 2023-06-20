#include "pch.h"
#include "CScript_VespeneSmoke.h"

#include "define_SC.h"
#include <Engine/CAnimator2D.h>

#include <Engine/CTransform.h>

CScript_VespeneSmoke::CScript_VespeneSmoke(const string& _strKey)
	:CScript_SCEntity(_strKey)
{
}

CScript_VespeneSmoke::~CScript_VespeneSmoke()
{
}

void CScript_VespeneSmoke::init()
{
	CScript_SCEntity::init();

	Animator2D()->Play(SC::strKey_Anim::Neutral::VESPENE_SMOKE_1, eANIM_LOOPMODE::NORMAL_LOOP, false);
}	

void CScript_VespeneSmoke::tick()
{
}
