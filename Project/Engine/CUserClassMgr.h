#pragma once
#include "CSingleton.h"

#include "struct.h"
#include "ptr.h"

//Script 프로젝트에서 생성되는 유저 클래스들을 관리하는 클래스
//현재 관리하는 클래스 목록
//1. CScript
//2. CComputeShader
//3. CParticleSystem


class CScript;
class CTilemap;
class CParticleSystem;
class CComputeShader;
class CUserClassMgr
	: public CSingleton<CUserClassMgr>
{
	SINGLETON(CUserClassMgr);

	//============================== SCRIPTS =====================================
private:
	std::unordered_map <std::string_view, std::function<CScript*(const string_view)>> m_umapScript;

public:
	void AddBaseScript(const std::string_view _strKey, std::function<CScript*(const string_view)> _FuncConstructor);
	CScript* GetNewScript(const std::string_view _strKey);


	// ===================================== Compute Shader ==============================
private:
	std::unordered_map <std::string_view, std::function<Ptr<CComputeShader> ()>> m_umapCS;
public:
	//생성자 주소만 가지고 있다가 GetCS를 요청 시 CResMgr에 등록해서 사용
	void AddBaseCS(const std::string_view _strKey, std::function<Ptr<CComputeShader> ()> _FuncConstructor);
	Ptr<CComputeShader> GetNewCS(const std::string_view _strKey);


private:
	std::unordered_map <std::string_view, std::function<CTilemap* ()>>			m_umapTilemap;
public:
	void AddBaseTilemap(const std::string_view _strKey, std::function<CTilemap* ()>);
	CTilemap* GetNewTilemap(const std::string_view _strKey);

private:
	std::unordered_map <std::string_view, std::function<CParticleSystem* ()>>	m_umapParticle;


};

inline void CUserClassMgr::AddBaseScript(const std::string_view _strKey, std::function<CScript*(const string_view)> _FuncConstructor)
{
	m_umapScript.insert(std::make_pair(_strKey, _FuncConstructor));
}

inline void CUserClassMgr::AddBaseCS(const std::string_view _strKey, std::function<Ptr<CComputeShader>()> _FuncConstructor)
{
	m_umapCS.insert(std::make_pair(_strKey, _FuncConstructor));
}

inline void CUserClassMgr::AddBaseTilemap(const std::string_view _strKey, std::function<CTilemap* ()> _FuncConstructor)
{
	m_umapTilemap.insert(std::make_pair(_strKey, _FuncConstructor));
}




