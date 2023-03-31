
#pragma once

#include "pch.h"
#include "CScriptMgrInitializer.h"

#include <Engine/CScriptMgr.h>


//Created Scripts
#include "CScript_Bullet.h"
#include "CScript_CameraMove.h"
#include "CScript_Marine.h"
#include "CScript_MouseCursor.h"
#include "CScript_SCGroundUnitFSM.h"
#include "CScript_TestObj.h"

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
		string strKey = string(SCRIPTS::MARINE);
		CScript* newScript = new CScript_Marine;
		newScript->SetName("CScript_Marine");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::MOUSECURSOR);
		CScript* newScript = new CScript_MouseCursor;
		newScript->SetName("CScript_MouseCursor");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::SCGROUNDUNITFSM);
		CScript* newScript = new CScript_SCGroundUnitFSM;
		newScript->SetName("CScript_SCGroundUnitFSM");
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::TESTOBJ);
		CScript* newScript = new CScript_TestObj;
		newScript->SetName("CScript_TestObj");
		pMgr->AddBaseScript(strKey, newScript);
	}
}