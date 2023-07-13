#include "pch.h"

#include "FSM.h"

#include "GameObject.h"

FSM::FSM(const string_view _strKey, UINT _uStateID)
	: Script(_strKey)
	, m_uStateID(_uStateID)
{
}

FSM::FSM(const FSM& _other)
	: Script(_other)
	, m_uStateID(_other.m_uStateID)
{
}

FSM::~FSM()
{
}


void FSM::SetHolder(ScriptHolder* _pScriptHolder)
{
	//부모의 SetHolder 호출 후
	Script::SetHolder(_pScriptHolder);

	assert(nullptr != _pScriptHolder);

	//자신은 FSM으로 추가
	_pScriptHolder->AddFSM(this);
}



