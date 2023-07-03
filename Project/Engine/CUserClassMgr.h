#pragma once
#include "CSingleton.h"

#include "struct.h"

//Script 프로젝트에서 생성되는 유저 클래스들을 관리하는 클래스
//현재 관리하는 클래스 목록
//1. CScript
//2. CParticleSystem
//3. CComputeShader
class CScript;
class CUserClassMgr
	: public CSingleton<CUserClassMgr>
{
	SINGLETON(CUserClassMgr);

protected:
	std::vector <std::function<CScript* ()>> m_vecScriptConstructor;

public:
	void AddBaseScript(std::function<CScript* ()> _funcConstructor) { m_vecScriptConstructor.push_back(_funcConstructor); }
	CScript* GetNewScript(size_t _iScriptID);
};

