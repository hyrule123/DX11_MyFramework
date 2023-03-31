#pragma once

#include <Engine/CScriptMgr.h>
#include "strKeyScript.h"

class CScript;
class CScriptMgrInitializer
{
public:
	CScriptMgrInitializer() = default;
	virtual ~CScriptMgrInitializer() = default;
public:
	void init();
};
