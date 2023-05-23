#include "pch.h"

#include "CFSM.h"

#include "CGameObject.h"

CFSM::CFSM(UINT _uStateID)
	: m_uStateID(_uStateID)
{
}

CFSM::CFSM(const CFSM& _other)
	: CScript(_other)
	, m_uStateID(_other.m_uStateID)
{
}

CFSM::~CFSM()
{
}



void CFSM::SetHolder(CScriptHolder* _pScriptHolder)
{
	CScript::SetHolder(_pScriptHolder);

	assert(nullptr != _pScriptHolder);

	_pScriptHolder->AddFSM(this);
}



