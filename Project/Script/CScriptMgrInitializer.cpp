
#pragma once

#include "pch.h"
#include "CScriptMgrInitializer.h"

#include <Engine/CScriptMgr.h>


//Created Scripts
#include "CScript_Bullet.h"
#include "CScript_CameraMove.h"
#include "CScript_Marine.h"
#include "CScript_MouseCursor.h"
#include "CScript_SCUnitVital.h"
#include "CScript_TestObj.h"

void CScriptMgrInitializer::init()
{
	CScriptMgr* pMgr = CScriptMgr::GetInst();
	{
		string strKey = string(SCRIPTS::BULLET);
		CScript* newScript = new CScript_Bullet;
		newScript->SetName("CScript_Bullet.h");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::CAMERAMOVE);
		CScript* newScript = new CScript_CameraMove;
		newScript->SetName("CScript_CameraMove.h");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::MARINE);
		CScript* newScript = new CScript_Marine;
		newScript->SetName("CScript_Marine.h");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::MOUSECURSOR);
		CScript* newScript = new CScript_MouseCursor;
		newScript->SetName("CScript_MouseCursor.h");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::SCUNITVITAL);
		CScript* newScript = new CScript_SCUnitVital;
		newScript->SetName("CScript_SCUnitVital.h");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::TESTOBJ);
		CScript* newScript = new CScript_TestObj;
		newScript->SetName("CScript_TestObj.h");
		pMgr->AddBaseScript(strKey, newScript);
	}
}