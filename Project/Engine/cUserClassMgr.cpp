#include "pch.h"
#include "cUserClassMgr.h"

#include "cResMgr.h"

#include "IScript.h"
#include "cComputeShader.h"
#include "IRenderer.h"


cUserClassMgr::cUserClassMgr()
{


}

cUserClassMgr::~cUserClassMgr()
{
}


IScript* cUserClassMgr::GetNewScript(const std::string_view _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);
	if (iter == m_umapScript.end())
		return nullptr;

	IScript* pScript = iter->second(_strKey);
	return pScript;
}

void cUserClassMgr::AddComponentConstructor(const std::string_view _strKey, std::function<IComponent* ()>)
{
}

IComponent* cUserClassMgr::GetComponent(const std::string_view _strKey)
{
	return nullptr;
}






