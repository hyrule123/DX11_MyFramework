#include "pch.h"
#include "CScript_Mineral.h"

#include <Engine/S_H_SCUnitGround.hlsli>

CScript_Mineral::CScript_Mineral(const string& _strKey)
	: CScript_SCEntity(_strKey)
	, m_eMineralAtlasType()
	, m_iMineralMax(5000)
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
//#define MTRL_SCALAR_MINERAL_TEXINDEX	  MTRLDATA_PARAM_SCALAR(INT, 2)
//현재 미네랄을 얼마나 소모했는지
//#define MTRL_SCALAR_MINERAL_REMAINING     MTRLDATA_PARAM_SCALAR(INT, 3)
	
	int WearLevel = (int)(((float)(m_iMineralMax - m_iCurMineral) / (float)m_iMineralMax) * 4.f);
	if (4 <= WearLevel)
		WearLevel = 3;
	
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_MINERAL_WEAR_LEVEL, &WearLevel);
}
