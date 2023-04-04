#pragma once
#include "Singleton.h"


//CScript 프로젝트에서 상속받아 사용하는 용도(Engine단에서 인스턴스를 만들지 않음)
class CScript;
class CScriptMgr
{
	DECLARE_SINGLETON(CScriptMgr);

protected:
	unordered_map<string, CScript*> m_umapScriptName;
	unordered_map<std::type_index, CScript*> m_umapScriptTypeIdx;

public:
	void AddBaseScript(const string& _strKey, CScript* _pScript);
	CScript* GetNewScript(const string& _strKey);
	CScript* GetNewScript(std::type_index _typeIdx);
};

