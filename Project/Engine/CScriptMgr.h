#pragma once
#include "CSingleton.h"


//CScript 프로젝트에서 상속받아 사용하는 용도(Engine단에서 인스턴스를 만들지 않음)
class CScript;
class CScriptMgr
	: public CSingleton<CScriptMgr>
{
	SINGLETON(CScriptMgr);

protected:
	map<string, CScript*> m_mapScript;

public:
	void AddBaseScript(const string& _strKey, CScript* _pScript);
	CScript* GetNewScript(const string& _strKey);
};

