#include "pch.h"

#include "cFSM.h"

#include "cGameObject.h"
#include "cScriptHolder.h"

cFSM::cFSM(const string_view _strKey, UINT _uStateID)
	: IScript(_strKey)
	, m_uStateID(_uStateID)
{
}

cFSM::cFSM(const cFSM& _other)
	: IScript(_other)
	, m_uStateID(_other.m_uStateID)
{
}

cFSM::~cFSM()
{
}


void cFSM::SetHolder(cScriptHolder* _pScriptHolder)
{
	//부모의 SetHolder 호출 후
	IScript::SetHolder(_pScriptHolder);

	assert(nullptr != _pScriptHolder);

	//자신은 cFSM으로 추가
	_pScriptHolder->AddFSM(this);
}



