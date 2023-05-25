
#pragma once

#include "pch.h"
#include "CScriptMgrInitializer.h"

#include <Engine/CScriptMgr.h>


//Created Scripts
#include "CScript_FSM_Move_Ground.h"
#include "CScript_FSM_AttackEnd.h"
#include "CScript_FSM_AttackBegin.h"
#include "CScript_CameraMove.h"
#include "CScript_FSM_Attack.h"
#include "CScript_FSM_Death.h"
#include "CScript_FSM_Idle.h"
#include "CScript_FSM_Move_Hover.h"
#include "CScript_Marine.h"
#include "CScript_MouseCursor.h"
#include "CScript_SCEntity.h"
#include "CScript_SCUnitVital.h"
#include "CScript_TestObj.h"

void CScriptMgrInitializer::init()
{
	CScriptMgr* pMgr = CScriptMgr::GetInst();
	{
		string strKey = string(SCRIPTS::FSM_MOVE_GROUND);
		CScript* newScript = new CScript_FSM_Move_Ground(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::FSM_ATTACKEND);
		CScript* newScript = new CScript_FSM_AttackEnd(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::FSM_ATTACKBEGIN);
		CScript* newScript = new CScript_FSM_AttackBegin(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::CAMERAMOVE);
		CScript* newScript = new CScript_CameraMove(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::FSM_ATTACK);
		CScript* newScript = new CScript_FSM_Attack(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::FSM_DEATH);
		CScript* newScript = new CScript_FSM_Death(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::FSM_IDLE);
		CScript* newScript = new CScript_FSM_Idle(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::FSM_MOVE_HOVER);
		CScript* newScript = new CScript_FSM_Move_Hover(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::MARINE);
		CScript* newScript = new CScript_Marine(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::MOUSECURSOR);
		CScript* newScript = new CScript_MouseCursor(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::SCENTITY);
		CScript* newScript = new CScript_SCEntity(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::SCUNITVITAL);
		CScript* newScript = new CScript_SCUnitVital(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(SCRIPTS::TESTOBJ);
		CScript* newScript = new CScript_TestObj(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
}