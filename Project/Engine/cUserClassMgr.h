#pragma once
#include "Singleton.h"

#include "struct.h"
#include "Ptr.h"

//cScript 프로젝트에서 생성되는 유저 클래스들을 관리하는 클래스
//현재 관리하는 클래스 목록
//1. cScript
//2. cComputeShader
//3. cParticleSystem

#define GET_NEW_SCRIPT(_Type) static_cast<_Type*>(cUserClassMgr::GetNewScript(strKey_Script::_Type))

class cScript;
class cTilemap;
class cParticleSystem;
class cComputeShader;
class cUserClassMgr
	: public Singleton<cUserClassMgr>
{
	SINGLETON(cUserClassMgr);


	//============================== SCRIPTS =====================================
private:
	std::unordered_map <std::string_view, std::function<cScript*(const string_view)>> m_umapScript;

public:
	void AddBaseScript(const std::string_view _strKey, std::function<cScript*(const string_view)> _FuncConstructor);
	cScript* GetNewScript(const std::string_view _strKey);


	// ===================================== Compute Shader ==============================
private:
	std::unordered_map <std::string_view, std::function<Ptr<cComputeShader> ()>> m_umapCS;
public:
	//생성자 주소만 가지고 있다가 GetCS를 요청 시 cResMgr에 등록해서 사용
	void AddBaseCS(const std::string_view _strKey, std::function<Ptr<cComputeShader> ()> _FuncConstructor);
	Ptr<cComputeShader> GetNewCS(const std::string_view _strKey);


private:
	static std::unordered_map <std::string_view, std::function<cParticleSystem* ()>>	m_umapParticle;
};


inline void cUserClassMgr::AddBaseScript(const std::string_view _strKey, std::function<cScript*(const string_view)> _FuncConstructor)
{
	m_umapScript.insert(std::make_pair(_strKey, _FuncConstructor));
}

inline void cUserClassMgr::AddBaseCS(const std::string_view _strKey, std::function<Ptr<cComputeShader>()> _FuncConstructor)
{
	m_umapCS.insert(std::make_pair(_strKey, _FuncConstructor));
}


