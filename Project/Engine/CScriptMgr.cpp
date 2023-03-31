#include "pch.h"
#include "CScriptMgr.h"
#include "CScript.h"


CScriptMgr::CScriptMgr()
{
}

CScriptMgr::~CScriptMgr()
{
	for (const auto& iter : m_umapScript)
	{
		DESTRUCTOR_DELETE(iter.second);
	}
}


void CScriptMgr::AddBaseScript(const string& _strKey, CScript* _pScript)
{
	if (nullptr == _pScript || _strKey.empty() || m_umapScript.end() != m_umapScript.find(_strKey))
		return;

	m_umapScript.insert(std::make_pair(_strKey, _pScript));
}

CScript* CScriptMgr::GetNewScript(const string& _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);

	if (iter == m_umapScript.end())
		return nullptr;

	return iter->second->Clone();
}
