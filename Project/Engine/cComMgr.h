#pragma once
#include "Singleton.h"

#include "struct.h"
#include "Ptr.h"
//User 단계에서 생성되는 클래스들을 등록, 관리하는 클래스


class IComponent;
class cComMgr
	: public Singleton<cComMgr>
{
	SINGLETON(cComMgr);


//	//============================== SCRIPTS =====================================
//private:
//	std::unordered_map <std::string_view, std::function<IScript*()>> m_umapScript;
//
//public:
//	void AddScriptConstructor(const std::string_view _strKey, std::function<IScript*()> _FuncConstructor);
//	IScript* GetNewScript(const std::string_view _strKey);
//	//====================================================================================


	// ==================================== Components ============================================
private:
	std::unordered_map <std::string_view, std::function<IComponent* ()>>	m_umapComConstructor;
	std::unordered_map <std::type_index, const std::string_view>			m_umapComName;
	
public:	
	template <typename T>
	void AddComConstructor(const std::string_view _strKey);
	IComponent* GetNewCom(const std::string_view _strKey);
	const std::string_view GetComName(std::type_index _TypeIdx);
	// =================================================================================================
};

//
//inline void cComMgr::AddScriptConstructor(const std::string_view _strKey, std::function<IScript*()> _FuncConstructor)
//{
//	assert(_FuncConstructor);
//	m_umapScript.insert(std::make_pair(_strKey, _FuncConstructor));
//}


template <typename T>
inline void cComMgr::AddComConstructor(const std::string_view _strKey)
{
	static_assert(std::is_base_of_v<IComponent, T>);
	m_umapComName.insert(std::make_pair(std::type_index(typeid(T)), _strKey));
	m_umapComConstructor.insert(std::make_pair(_strKey, 
		[]()->T*
		{
			return new T;
		}
		));
}



