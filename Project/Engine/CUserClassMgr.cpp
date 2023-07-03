#include "pch.h"
#include "CUserClassMgr.h"
#include "CScript.h"


CUserClassMgr::CUserClassMgr()
{
}

CUserClassMgr::~CUserClassMgr()
{
}

CScript* CUserClassMgr::GetNewScript(size_t _iScriptID)
{
	if (_iScriptID >= m_vecScriptConstructor.size())
		return nullptr;

	return m_vecScriptConstructor[_iScriptID]();
}


