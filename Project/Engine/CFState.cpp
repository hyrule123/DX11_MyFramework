#include "pch.h"

#include "CFState.h"

#include "CFStateMgr.h"

CFState::CFState(std::type_index _typeIdx, UINT _eMyState)
	: CScript(_typeIdx)
	, m_eMyState(_eMyState)
	, m_pFStateMgr()
{
}

CFState::CFState(const CFState& _other)
	: CScript(_other)
	, m_eMyState(_other.m_eMyState)
	, m_pFStateMgr()
{
}

CFState::~CFState()
{
}

