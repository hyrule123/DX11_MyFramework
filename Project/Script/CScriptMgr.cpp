
#pragma once

#include "pch.h"
#include "CScriptMgr.h"


//Created Scripts
#include "CScript_Bullet.h"
#include "CScript_CameraMove.h"
#include "CScript_Debug.h"
#include "CScript_MouseCursor.h"
#include "CScript_SCGroundUnitMove.h"
#include "CScript_SCGroundUnitBase.h"
#include "CScript_SCGroundUnitIdle.h"
#include "CScript_TestObj.h"



CScriptMgr::CScriptMgr() {}
CScriptMgr::~CScriptMgr()
{
	for(const auto& iter : m_umapScript)
	{
		DESTRUCTOR_DELETE(iter.second);
	}
}


void CScriptMgr::init()
{
	m_umapScript[std::string(SCRIPTS::BULLET)] = new CScript_Bullet;
	m_umapScript[std::string(SCRIPTS::CAMERAMOVE)] = new CScript_CameraMove;
	m_umapScript[std::string(SCRIPTS::DEBUG)] = new CScript_Debug;
	m_umapScript[std::string(SCRIPTS::MOUSECURSOR)] = new CScript_MouseCursor;
	m_umapScript[std::string(SCRIPTS::SCGROUNDUNITMOVE)] = new CScript_SCGroundUnitMove;
	m_umapScript[std::string(SCRIPTS::SCGROUNDUNITBASE)] = new CScript_SCGroundUnitBase;
	m_umapScript[std::string(SCRIPTS::SCGROUNDUNITIDLE)] = new CScript_SCGroundUnitIdle;
	m_umapScript[std::string(SCRIPTS::TESTOBJ)] = new CScript_TestObj;
}

CScript* CScriptMgr::GetNewScript(const string& _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);

	if (iter == m_umapScript.end())
		return nullptr;

	return iter->second->Clone();
}
