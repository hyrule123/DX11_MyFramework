#include "pch.h"
#include "cFSM_Move_Base.h"

#include "cScript_SCEntity.h"

cFSM_Move_Base::cFSM_Move_Base(const string_view _strKey, UINT _eMyState, SC::eUNIT_MOVE_TYPE _eMoveType)
	: I_FSM(_strKey, _eMyState)
	, m_eMoveType(_eMoveType)
	, m_fSpeed(10.f)
	, m_fTurnSpeed(0.5f)
	, m_bArrived(true)
{
}

cFSM_Move_Base::~cFSM_Move_Base()
{
}

void cFSM_Move_Base::Init()
{
	cScript_SCEntity* pEntity = static_cast<cScript_SCEntity*>(ScriptHolder()->FindScript("cScript_SCEntity"));
	assert(nullptr != pEntity);

	pEntity->SetMoveType(m_eMoveType);
}



