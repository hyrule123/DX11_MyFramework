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
	std::unordered_map <std::string_view, std::function<CScript* ()>> m_umapScript;

public:
	void AddBaseScript(const std::string_view _strKey, std::function<CScript* ()> _FuncConstructor);
	CScript* GetNewScript(const std::string_view _strKey);
};

inline void CUserClassMgr::AddBaseScript(const std::string_view _strKey, std::function<CScript* ()> _FuncConstructor)
{
	m_umapScript.insert(std::make_pair(_strKey, _FuncConstructor));
}

inline CScript* CUserClassMgr::GetNewScript(const std::string_view _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);
	if (iter == m_umapScript.end())
		return nullptr;

	//함수 호출 시 동적할당해서 반환해줄 것임.
	return iter->second();
}
