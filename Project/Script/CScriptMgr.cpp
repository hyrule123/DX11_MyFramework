#include "pch.h"
#include "CScriptMgr.h"

#include <Engine/CScript.h>

//이 아래에 모든 스크립트를 include

CScriptMgr::CScriptMgr() {}

CScriptMgr::~CScriptMgr()
{
	for (const auto& iter : m_umapScript)
	{
		DESTRUCTOR_DELETE(iter.second);
	}
}

void CScriptMgr::init() 
{ 
	//여기에 CodeGenerator 프로그램으로 코드를 생성해준다.
}
