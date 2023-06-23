#include "pch.h"
#include "CScript_FSM_Building_Prod.h"

#include <Engine/S_H_SCUnitGround.hlsli>
#include <Engine/CKeyMgr.h>

#include <Engine/CTimeMgr.h>
#include <Engine/CRandMgr.h>

CScript_FSM_Building_Prod::CScript_FSM_Building_Prod(const string_view _strKey)
	: CScript_FSM_Idle(_strKey)
	, m_bIsProducing(false)
	, m_bIsFlickerOn()
	, m_fFlickerTime()
{
}

CScript_FSM_Building_Prod::~CScript_FSM_Building_Prod()
{
}

void CScript_FSM_Building_Prod::tick()
{
	if (m_bIsProducing)
	{
		m_fFlickerTime -= DELTA_TIME;
		if (m_fFlickerTime < 0.f)
		{
			m_bIsFlickerOn = !m_bIsFlickerOn;
			UINT randVal = 0u;
			if (m_bIsFlickerOn)
				randVal = 9u;
			else
				randVal = 2u;

			//꺼져있을때 = 최대 0.7 ~ 0.14
			randVal = CRandMgr::GetInst()->GetRand<UINT>(1u, randVal);

			m_fFlickerTime = 0.02f * (float)randVal;
		}
	}


	if (KEY_DOWN(eKEY::P))
	{
		m_bIsProducing = !m_bIsProducing;
	}

	//재질 데이터에 전송
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (INT32)eMTRL_SCALAR_FLAG_BUILDING_STRUCTURE::ENABLE_PROD_TEX, m_bIsFlickerOn);
}

