#include "pch.h"

#include "CFState.h"

#include "CGameObject.h"

#include "CFStateMgr.h"

CFState::CFState(UINT _eMyState)
	: m_eMyState(_eMyState)
	, m_pFStateMgr()
{
}

CFState::CFState(const CFState& _other)
	: m_eMyState(_other.m_eMyState)
	, m_pFStateMgr()
{
}

CFState::~CFState()
{
}

CFState* CFState::Transition(UINT _eState)
{
	if (m_pFStateMgr)
		return m_pFStateMgr->Transition(_eState);

	return nullptr;
}

