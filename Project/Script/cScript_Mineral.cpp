
#include "pch.h"

#include <Engine/EventDispatcher.h>

#include "cScript_Mineral.h"

constexpr int MINERAL_WEAR_LEVEL_3 = 250;
constexpr int MINERAL_WEAR_LEVEL_2 = 500;
constexpr int MINERAL_WEAR_LEVEL_1 = 750;

cScript_Mineral::cScript_Mineral(const string_view _strKey)
	: cScript_SCEntity(_strKey)
	, m_eMineralAtlasType()
	, m_iMineralLeft(1500)
{
}

cScript_Mineral::~cScript_Mineral()
{
}

void cScript_Mineral::init()
{
	cScript_SCEntity::init();
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_TEXINDEX, &m_eMineralAtlasType);
}

void cScript_Mineral::tick()
{
	int iWearLevel = 0;

	if (m_iMineralLeft < MINERAL_WEAR_LEVEL_3)
		iWearLevel = 3;
	else if (m_iMineralLeft < MINERAL_WEAR_LEVEL_2)
		iWearLevel = 2;
	else if (m_iMineralLeft < MINERAL_WEAR_LEVEL_1)
		iWearLevel = 1;

	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_WEAR_LEVEL, &iWearLevel);

	if (m_iMineralLeft < 0)
		EventDispatcher::DestroyGameObj(GetOwner());
}
