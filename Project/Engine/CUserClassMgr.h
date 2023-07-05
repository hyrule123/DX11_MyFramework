#pragma once
#include "CSingleton.h"

#include "struct.h"
#include "ptr.h"

//Script 프로젝트에서 생성되는 유저 클래스들을 관리하는 클래스
//현재 관리하는 클래스 목록
//1. CScript
//2. CParticleSystem
//3. CComputeShader

class CScript;
class CParticleSystem;
class CComputeShader;
class CUserClassMgr
	: public CSingleton<CUserClassMgr>
{
	SINGLETON(CUserClassMgr);

private:
	std::unordered_map <std::string_view, std::function<CScript* ()>> m_umapScript;

public:
	void AddBaseScript(const std::string_view _strKey, std::function<CScript* ()> _FuncConstructor);
	CScript* GetNewScript(const std::string_view _strKey);

private:
	std::unordered_map <std::string_view, std::function<Ptr<CComputeShader> ()>> m_umapCS;
public:
	//생성자 주소만 가지고 있다가 GetCS를 요청 시 CResMgr에 등록해서 사용
	void AddBaseCS(const std::string_view _strKey, std::function<Ptr<CComputeShader> ()> _FuncConstructor);
	Ptr<CComputeShader> GetNewCS(const std::string_view _strKey);



private:
	std::unordered_map <std::string_view, std::function<CParticleSystem* ()>> m_umapParticle;


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

inline void CUserClassMgr::AddBaseCS(const std::string_view _strKey, std::function<Ptr<CComputeShader>()> _FuncConstructor)
{
	m_umapCS.insert(std::make_pair(_strKey, _FuncConstructor));
}


