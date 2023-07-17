#include "pch.h"

#include "I_FSM.h"

#include "cGameObject.h"
#include "cScriptHolder.h"

I_FSM::I_FSM(const string_view _strKey, UINT _uStateID)
	: IScript(_strKey)
	, m_uStateID(_uStateID)
{
}

I_FSM::I_FSM(const I_FSM& _other)
	: IScript(_other)
	, m_uStateID(_other.m_uStateID)
{
}

I_FSM::~I_FSM()
{
}


void I_FSM::SetHolder(cScriptHolder* _pScriptHolder)
{
	//부모의 SetHolder 호출 후
	IScript::SetHolder(_pScriptHolder);

	assert(nullptr != _pScriptHolder);

	//자신은 I_FSM으로 추가
	_pScriptHolder->AddFSM(this);
}



