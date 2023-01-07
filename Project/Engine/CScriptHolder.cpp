#include "pch.h"
#include "CScriptHolder.h"

#include "CScript.h"

bool CScriptHolder::AddScript(CScript* _pScript)
{
	if (nullptr == _pScript)
		return false;

	std::type_index type = _pScript->GetTypeIndex();

	//스크립트 중복 방지
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (type == m_vecScript[i]->GetTypeIndex())
		{
			delete _pScript;
			return false;
		}
	}

	m_vecScript.push_back(_pScript);
	_pScript->SetOwner(GetOwner());

	return true;
}

void CScriptHolder::init()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->init();
	}
}

void CScriptHolder::tick()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->tick();
	}
}

CScriptHolder::CScriptHolder()
	: CComponent(eCOMPONENT_TYPE::SCRIPT)
{
}

CScriptHolder::CScriptHolder(const CScriptHolder& _other)
	: CComponent(_other)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript.push_back(_other.m_vecScript[i]->Clone());
		m_vecScript[i]->SetOwner(GetOwner());
	}
}

CScriptHolder::~CScriptHolder()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (nullptr != m_vecScript[i])
			delete m_vecScript[i];
	}
}
