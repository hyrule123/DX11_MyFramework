#include "pch.h"
#include "CFState_SCUnit_Move_Base.h"

#include "CSC_Entity.h"

CFState_SCUnit_Move_Base::CFState_SCUnit_Move_Base(UINT _eMyState, eSCUNIT_MOVE_TYPE _eMoveType)
	: CFSM(_eMyState)
	, m_eMoveType(_eMoveType)
	, m_fSpeed(10.f)
	, m_fTurnSpeed(0.5f)
	, m_bArrived(true)
{
}

CFState_SCUnit_Move_Base::~CFState_SCUnit_Move_Base()
{
}

void CFState_SCUnit_Move_Base::init()
{
	static_cast<CSC_Entity*>(GetFStateMgr())->SetMoveType(m_eMoveType);
}


