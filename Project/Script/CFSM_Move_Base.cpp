#include "pch.h"
#include "CFSM_Move_Base.h"

#include "CScript_SCEntity.h"

CFSM_Move_Base::CFSM_Move_Base(const string& _strKey, UINT _eMyState, eSCUNIT_MOVE_TYPE _eMoveType)
	: CFSM(_strKey, _eMyState)
	, m_eMoveType(_eMoveType)
	, m_fSpeed(10.f)
	, m_fTurnSpeed(0.5f)
	, m_bArrived(true)
{
}

CFSM_Move_Base::~CFSM_Move_Base()
{
}

void CFSM_Move_Base::init()
{
	CScript_SCEntity* pEntity = static_cast<CScript_SCEntity*>(ScriptHolder()->FindScript("CScript_SCEntity"));
	assert(nullptr != pEntity);

	pEntity->SetMoveType(m_eMoveType);
}



