#include "pch.h"
#include "CScript_FSM_Building_Prod.h"

#include <Engine/S_H_SCUnitGround.hlsli>
#include <Engine/CKeyMgr.h>

CScript_FSM_Building_Prod::CScript_FSM_Building_Prod(const string& _strKey)
	: CScript_FSM_Idle(_strKey)
	, m_bIsProducing(false)
{
}

CScript_FSM_Building_Prod::~CScript_FSM_Building_Prod()
{
}

void CScript_FSM_Building_Prod::tick()
{
	if (KEY_DOWN(eKEY::P))
	{
		m_bIsProducing = !m_bIsProducing;
	}

	//재질 데이터에 전송
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_FLAG_BUILDING_STRUCTURE::ENABLE_PROD_TEX, m_bIsProducing);
}

