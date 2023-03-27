#include "pch.h"
#include "CFSM.h"

#include "CFSMMgr.h"

CFSM::CFSM(std::type_index _typeIdx, UINT _eMyState)
	: CScript(_typeIdx)
	, m_eMyState(_eMyState)
{
}

CFSM::~CFSM()
{
}

