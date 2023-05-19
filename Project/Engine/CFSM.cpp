#include "pch.h"

#include "CFSM.h"

#include "CGameObject.h"

#include "CFSM_Mgr.h"

CFSM::CFSM(UINT _eMyState)
	: m_eMyState(_eMyState)
	, m_pFStateMgr()
{
}

CFSM::CFSM(const CFSM& _other)
	: m_eMyState(_other.m_eMyState)
	, m_pFStateMgr()
{
}

CFSM::~CFSM()
{
}

CFSM* CFSM::Transition(UINT _eState)
{
	if (m_pFStateMgr)
		return m_pFStateMgr->Transition(_eState);

	return nullptr;
}

