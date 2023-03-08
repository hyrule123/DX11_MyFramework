#pragma once


class CScript;
class CScriptMgr
	: public CSingleton<CScriptMgr>
{
	SINGLETON(CScriptMgr);

private:
	unordered_map<string, CScript*> m_umapScript;

public:
	void init();

};

