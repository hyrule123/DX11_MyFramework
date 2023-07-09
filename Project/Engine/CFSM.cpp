#include "pch.h"

#include "CFSM.h"

#include "CGameObject.h"

CFSM::CFSM(const string_view _strKey, UINT _uStateID)
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
	//부모의 SetHolder 호출 후
	CScript::SetHolder(_pScriptHolder);

	assert(nullptr != _pScriptHolder);

	//자신은 FSM으로 추가
	_pScriptHolder->AddFSM(this);
}



