#include "pch.h"

#include "CFSM.h"

#include "CGameObject.h"

CFSM::CFSM(const string& _strKey, UINT _uStateID)
	: CScript(_strKey)
	, m_uStateID(_uStateID)
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



