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

	IScript* pScript = iter->second();
	return pScript;
}


IComponent* cUserClassMgr::GetNewComponent(const std::string_view _strKey)
{
	const auto& iter = m_umapComponent.find(_strKey);
	if (iter == m_umapComponent.end())
		return nullptr;

	IComponent* pScript = iter->second();
	pScript->SetKey(_strKey);
	return pScript;
}






