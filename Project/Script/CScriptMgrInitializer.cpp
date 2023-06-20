
#pragma once

#include "pch.h"
#include "CScriptMgrInitializer.h"

#include <Engine/CScriptMgr.h>


//Created Scripts
#include "CScript_FSM_Move_Ground.h"
#include "CScript_CameraMove.h"
#include "CScript_FSM_Attack.h"
#include "CScript_FSM_Move_Hover.h"
#include "CScript_FSM_Building_Prod.h"
#include "CScript_FSM_Death.h"
#include "CScript_TilemapUnitLoader.h"
#include "CScript_FSM_Idle.h"
#include "CScript_Mineral.h"
#include "CScript_MouseCursor.h"
#include "CScript_SCEntity.h"
#include "CScript_SCUnitVital.h"
#include "CScript_TestObj.h"
#include "CScript_Vespene.h"
#include "CScript_VespeneSmoke.h"

void CScriptMgrInitializer::init()
{
	CScriptMgr* pMgr = CScriptMgr::GetInst();
	{
		string strKey = string(strKey_SCRIPT::FSM_MOVE_GROUND);
		CScript* newScript = new CScript_FSM_Move_Ground(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::CAMERAMOVE);
		CScript* newScript = new CScript_CameraMove(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::FSM_ATTACK);
		CScript* newScript = new CScript_FSM_Attack(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::FSM_MOVE_HOVER);
		CScript* newScript = new CScript_FSM_Move_Hover(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::FSM_BUILDING_PROD);
		CScript* newScript = new CScript_FSM_Building_Prod(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::FSM_DEATH);
		CScript* newScript = new CScript_FSM_Death(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::TILEMAPUNITLOADER);
		CScript* newScript = new CScript_TilemapUnitLoader(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::FSM_IDLE);
		CScript* newScript = new CScript_FSM_Idle(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::MINERAL);
		CScript* newScript = new CScript_Mineral(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::MOUSECURSOR);
		CScript* newScript = new CScript_MouseCursor(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::SCENTITY);
		CScript* newScript = new CScript_SCEntity(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::SCUNITVITAL);
		CScript* newScript = new CScript_SCUnitVital(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::TESTOBJ);
		CScript* newScript = new CScript_TestObj(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::VESPENE);
		CScript* newScript = new CScript_Vespene(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
	{
		string strKey = string(strKey_SCRIPT::VESPENESMOKE);
		CScript* newScript = new CScript_VespeneSmoke(strKey);
		pMgr->AddBaseScript(strKey, newScript);
	}
}