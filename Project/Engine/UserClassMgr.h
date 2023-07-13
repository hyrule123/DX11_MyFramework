#pragma once
#include "Singleton.h"

#include "struct.h"
#include "Ptr.h"

//Script 프로젝트에서 생성되는 유저 클래스들을 관리하는 클래스
//현재 관리하는 클래스 목록
//1. Script
//2. C_ComputeShader
//3. ParticleSystem

#define GET_NEW_SCRIPT(_Type) static_cast<_Type*>(UserClassMgr::GetNewScript(strKey_Script::_Type))

class Script;
class Tilemap;
class ParticleSystem;
class C_ComputeShader;
class UserClassMgr
{
	//============================== SCRIPTS =====================================
private:
	static std::unordered_map <std::string_view, std::function<Script*(const string_view)>> m_umapScript;

public:
	static void AddBaseScript(const std::string_view _strKey, std::function<Script*(const string_view)> _FuncConstructor);
	static Script* GetNewScript(const std::string_view _strKey);


	// ===================================== Compute Shader ==============================
private:
	static std::unordered_map <std::string_view, std::function<Ptr<C_ComputeShader> ()>> m_umapCS;
public:
	//생성자 주소만 가지고 있다가 GetCS를 요청 시 ResMgr에 등록해서 사용
	static void AddBaseCS(const std::string_view _strKey, std::function<Ptr<C_ComputeShader> ()> _FuncConstructor);
	static Ptr<C_ComputeShader> GetNewCS(const std::string_view _strKey);


private:
	static std::unordered_map <std::string_view, std::function<ParticleSystem* ()>>	m_umapParticle;


	UserClassMgr() = delete;
	~UserClassMgr() = delete;
};

inline void UserClassMgr::AddBaseScript(const std::string_view _strKey, std::function<Script*(const string_view)> _FuncConstructor)
{
	m_umapScript.insert(std::make_pair(_strKey, _FuncConstructor));
}

inline void UserClassMgr::AddBaseCS(const std::string_view _strKey, std::function<Ptr<C_ComputeShader>()> _FuncConstructor)
{
	m_umapCS.insert(std::make_pair(_strKey, _FuncConstructor));
}


