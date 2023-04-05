#include "pch.h"
#include "CFState_SCUnitMove.h"

#include "CSCEntity.h"

CFState_SCUnitMove::CFState_SCUnitMove(UINT _eMyState, eSCUNIT_MOVE_TYPE _eMoveType)
	: CFState(_eMyState)
	, m_eMoveType(_eMoveType)
	, m_fSpeed(10.f)
	, m_fTurnSpeed(0.5f)
	, m_bArrived(true)
{
}

CFState_SCUnitMove::~CFState_SCUnitMove()
{
}

void CFState_SCUnitMove::init()
{
	static_cast<CSCEntity*>(GetFStateMgr())->SetMoveType(m_eMoveType);
}


