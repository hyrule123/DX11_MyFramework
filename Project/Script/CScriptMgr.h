#pragma once

#include "strKeyScript.h"

class CScript;
class CScriptMgr
	: public CSingleton<CScriptMgr>
{
	SINGLETON(CScriptMgr);

public:
	void init();

private:
	unordered_map<string, CScript*> m_umapScript;
	
public:
	CScript* GetNewScript(const string& _strKey);

	
};

