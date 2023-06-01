
#pragma once

#include "pch.h"
#include "CScriptMgrInitializer.h"

#include <Engine/CScriptMgr.h>


//Created Scripts
#include "CScript_FSM_Move_Ground.h"
#include "CScript_CameraMove.h"
#include "CScript_MouseCursor.h"
#include "CScript_FSM_Attack.h"
#include "CScript_FSM_Death.h"
#include "CScript_SCEntity.h"
#include "CScript_FSM_Idle.h"
#include "CScript_FSM_Move_Hover.h"
#include "CScript_SCUnitVital.h"
#include "CScript_TestObj.h"

void CScriptMgrInitializer::init()
{
	CScriptMgr* pMgr = CScriptMgr::GetInst();
	{
		string strKey = string(strKey_SCRIPTS::FSM_MOVE_GROUND);
		CScript* newScript = new CScript_FSM_Move_Ground(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::CAMERAMOVE);
		CScript* newScript = new CScript_CameraMove(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::MOUSECURSOR);
		CScript* newScript = new CScript_MouseCursor(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::FSM_ATTACK);
		CScript* newScript = new CScript_FSM_Attack(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::FSM_DEATH);
		CScript* newScript = new CScript_FSM_Death(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::SCENTITY);
		CScript* newScript = new CScript_SCEntity(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::FSM_IDLE);
		CScript* newScript = new CScript_FSM_Idle(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::FSM_MOVE_HOVER);
		CScript* newScript = new CScript_FSM_Move_Hover(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::SCUNITVITAL);
		CScript* newScript = new CScript_SCUnitVital(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPTS::TESTOBJ);
		CScript* newScript = new CScript_TestObj(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
}