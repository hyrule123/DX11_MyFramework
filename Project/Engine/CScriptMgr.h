#pragma once
#include "CSingleton.h"

#include "struct.h"

#define GET_SCRIPT(T, strKey) static_cast<T*>(CScriptMgr::GetInst()->GetNewScript(strKey))

//CScript 프로젝트에서 상속받아 사용하는 용도(Engine단에서 인스턴스를 만들지 않음)
class CScript;
class CScriptMgr
	: public CSingleton<CScriptMgr>
{
	SINGLETON(CScriptMgr);

protected:
	map<string, CScript*, std::less<>> m_mapScript;

public:
	void AddBaseScript(const string_view _strKey, CScript* _pScript);
	CScript* GetNewScript(const string_view _strKey);
};

