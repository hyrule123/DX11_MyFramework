#include "pch.h"
#include "CScriptMgr.h"
#include "CScript.h"


CScriptMgr::CScriptMgr()
{
}

CScriptMgr::~CScriptMgr()
{
	for (const auto& iter : m_mapScript)
	{
		if (iter.second)
			delete iter.second;
	}
}

void CScriptMgr::AddBaseScript(const string& _strKey, CScript* _pScript)
{
	if (nullptr == _pScript || _strKey.empty())
		return;
	else if (m_mapScript.end() != m_mapScript.find(_strKey))
		return;

	m_mapScript.insert(std::make_pair(_strKey, _pScript));
}

CScript* CScriptMgr::GetNewScript(const string& _strKey)
{
	const auto& iter = m_mapScript.find(_strKey);

	if (iter == m_mapScript.end())
		return nullptr;

	return iter->second->Clone();
}

