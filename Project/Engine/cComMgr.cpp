#include "pch.h"
#include "cComMgr.h"

#include "cResMgr.h"

#include "IScript.h"
#include "cComputeShader.h"
#include "IRenderer.h"


cComMgr::cComMgr()
{


}

cComMgr::~cComMgr()
{
}


//IScript* cComMgr::GetNewScript(const std::string_view _strKey)
//{
//	const auto& iter = m_umapScript.find(_strKey);
//	if (iter == m_umapScript.end())
//		return nullptr;
//
//	IScript* pScript = iter->second();
//	return pScript;
//}


IComponent* cComMgr::GetNewCom(const std::string_view _strKey)
{
	const auto& iter = m_umapComConstructor.find(_strKey);
	if (iter == m_umapComConstructor.end())
		return nullptr;

	IComponent* pScript = iter->second();
	pScript->SetKey(_strKey);
	return pScript;
}

const std::string_view cComMgr::GetComName(const std::type_info& _typeid_T_)
{
	const auto& iter = m_umapComName.find(std::type_index(_typeid_T_));
	if (iter == m_umapComName.end())
		return "";
	return iter->second;
}






