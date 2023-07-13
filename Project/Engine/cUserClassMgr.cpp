#include "pch.h"
#include "cUserClassMgr.h"
#include "cScript.h"

#include "cComputeShader.h"
#include "cResMgr.h"

cUserClassMgr::cUserClassMgr()
{
}

cUserClassMgr::~cUserClassMgr()
{
}


cScript* cUserClassMgr::GetNewScript(const std::string_view _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);
	if (iter == m_umapScript.end())
		return nullptr;

	cScript* pScript = iter->second(_strKey);
	return pScript;
}

Ptr<cComputeShader> cUserClassMgr::GetNewCS(const std::string_view _strKey)
{
	//이미 등록되어 있는지 확인
	Ptr<cComputeShader> pCS = cResMgr::GetInst()->FindRes<cComputeShader>(_strKey);
	if (nullptr != pCS)
		return pCS;

	//없을 경우 직접 찾아서 반환
	const auto& iter = m_umapCS.find(_strKey);

	if (iter == m_umapCS.end())
		return nullptr;

	Ptr<cComputeShader> NewCS = iter->second();
	NewCS->SetKey(_strKey);

	//리소스 매니저에 리소스를 추가한다.
	cResMgr::GetInst()->AddRes(_strKey, NewCS);

	return NewCS;
}
