#include "pch.h"
#include "CScriptMgr.h"
#include "CScript.h"

DEFINITION_SINGLETON(CScriptMgr);

CScriptMgr::CScriptMgr()
{
}

CScriptMgr::~CScriptMgr()
{
	for (const auto& iter : m_umapScriptName)
	{
		DESTRUCTOR_DELETE(iter.second);
	}
}


void CScriptMgr::AddBaseScript(const string& _strKey, CScript* _pScript)
{
	if (nullptr == _pScript || _strKey.empty() || m_umapScriptName.end() != m_umapScriptName.find(_strKey))
		return;

	m_umapScriptName.insert(std::make_pair(_strKey, _pScript));
	m_umapScriptTypeIdx.insert(std::make_pair(_pScript->GetTypeIndex(), _pScript));
}

CScript* CScriptMgr::GetNewScript(const string& _strKey)
{
	const auto& iter = m_umapScriptName.find(_strKey);

	if (iter == m_umapScriptName.end())
		return nullptr;

	return iter->second->Clone();
}

CScript* CScriptMgr::GetNewScript(std::type_index _typeIdx)
{
	const auto& iter = m_umapScriptTypeIdx.find(_typeIdx);

	if (iter == m_umapScriptTypeIdx.end())
		return nullptr;

	return iter->second->Clone();
}
