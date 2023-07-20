#include "pch.h"
#include "cScript_FSM_Building_Prod.h"

#include <HLSL/S_H_SCUnitGround.hlsli>

#include <Engine/cKeyMgr.h>

#include <Engine/cTimeMgr.h>
#include <Engine/RandGen.h>

cScript_FSM_Building_Prod::cScript_FSM_Building_Prod(const string_view _strKey)
	: cScript_FSM_Idle(_strKey)
	, m_bIsProducing(false)
	, m_bIsFlickerOn()
	, m_fFlickerTime()
{
}

cScript_FSM_Building_Prod::~cScript_FSM_Building_Prod()
{
}

void cScript_FSM_Building_Prod::Tick()
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
			randVal = RandGen::GetRand<UINT>(1u, randVal);

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

