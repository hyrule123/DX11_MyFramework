
#pragma once

#include "pch.h"
#include "CScriptMgrInitializer.h"

#include <Engine/CScriptMgr.h>


//Created Scripts
#include "CScript_Bullet.h"
#include "CScript_CameraMove.h"
#include "CScript_Debug.h"
#include "CMouseCursorScriptBase.h"
#include "CScript_SCGroundUnitMove.h"
#include "CFStateMgr_SCGroundUnitBase.h"
#include "CFState_SCGroundUnitIdle.h"
#include "CScript_TestObj.h"

CScriptMgrInitializer::CScriptMgrInitializer()
{
}

CScriptMgrInitializer::~CScriptMgrInitializer()
{
}

void CScriptMgrInitializer::init()
{
	CScriptMgr* pMgr = CScriptMgr::GetInst();
	{
		string strKey = string(SCRIPTS::BULLET);
		CScript* newScript = new CScript_Bullet;
		newScript->SetName("CScript_Bullet");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::CAMERAMOVE);
		CScript* newScript = new CScript_CameraMove;
		newScript->SetName("CScript_CameraMove");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::DEBUG);
		CScript* newScript = new CScript_Debug;
		newScript->SetName("CScript_Debug");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::MOUSECURSOR);
		CScript* newScript = new CMouseCursorScriptBase;
		newScript->SetName("CMouseCursorScriptBase");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::SCGROUNDUNITMOVE);
		CScript* newScript = new CScript_SCGroundUnitMove;
		newScript->SetName("CScript_SCGroundUnitMove");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::SCGROUNDUNITBASE);
		CScript* newScript = new CFStateMgr_SCGroundUnitBase;
		newScript->SetName("CFStateMgr_SCGroundUnitBase");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::SCGROUNDUNITIDLE);
		CScript* newScript = new CFState_SCGroundUnitIdle;
		newScript->SetName("CFState_SCGroundUnitIdle");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::TESTOBJ);
		CScript* newScript = new CScript_TestObj;
		newScript->SetName("CScript_TestObj");
		pMgr->AddBaseScript(strKey, newScript);
	}
}
