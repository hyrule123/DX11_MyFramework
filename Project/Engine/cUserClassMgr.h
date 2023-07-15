#pragma once
#include "Singleton.h"

#include "struct.h"
#include "Ptr.h"

//IScript 프로젝트에서 생성되는 유저 클래스들을 관리하는 클래스
//현재 관리하는 클래스 목록
//1. IScript
//2. cComputeShader
//3. cCom_Renderer_Particle

#define GET_NEW_SCRIPT(_Type) static_cast<_Type*>(cUserClassMgr::GetNewScript(strKey_Script::_Type))


class cComputeShader;
class IComponent;
class IScript;



class cUserClassMgr
	: public Singleton<cUserClassMgr>
{
	SINGLETON(cUserClassMgr);


	//============================== SCRIPTS =====================================
private:
	std::unordered_map <std::string_view, std::function<IScript*(const string_view)>> m_umapScript;

public:
	void AddScriptConstructor(const std::string_view _strKey, std::function<IScript*(const string_view)> _FuncConstructor);
	IScript* GetNewScript(const std::string_view _strKey);
	//====================================================================================



	// ==================================== Components ============================================
private:
	static std::unordered_map <std::string_view, std::function<IComponent* ()>>	m_umapComponent;
	
public:
	void AddComponentConstructor(const std::string_view _strKey, std::function<IComponent* ()>);
	IComponent* GetComponent(const std::string_view _strKey);
	// =================================================================================================
};




inline void cUserClassMgr::AddScriptConstructor(const std::string_view _strKey, std::function<IScript*(const string_view)> _FuncConstructor)
{
	m_umapScript.insert(std::make_pair(_strKey, _FuncConstructor));
}




