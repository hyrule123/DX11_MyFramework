
#include "pch.h"

#include <Engine/EventDispatcher.h>

#include "CScript_Mineral.h"

#define MINERAL_WEAR_LEVEL_3 250
#define MINERAL_WEAR_LEVEL_2 500
#define MINERAL_WEAR_LEVEL_1 750

CScript_Mineral::CScript_Mineral(const string& _strKey)
	: CScript_SCEntity(_strKey)
	, m_eMineralAtlasType()
	, m_iMineralMax(1000)
{
	m_iCurMineral = m_iMineralMax;
}

CScript_Mineral::~CScript_Mineral()
{
}

void CScript_Mineral::init()
{
	CScript_SCEntity::init();
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &m_eMineralAtlasType);
}

void CScript_Mineral::tick()
{
	int iWearLevel = 0;

	if (m_iCurMineral < MINERAL_WEAR_LEVEL_3)
		iWearLevel = 3;
	else if (m_iCurMineral < MINERAL_WEAR_LEVEL_2)
		iWearLevel = 2;
	else if (m_iCurMineral < MINERAL_WEAR_LEVEL_1)
		iWearLevel = 1;

	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_WEAR_LEVEL, &iWearLevel);

	if (m_iCurMineral < 0)
		EventDispatcher::DestroyGameObj(GetOwner());
}
