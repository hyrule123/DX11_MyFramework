#include "pch.h"
#include "CUserClassMgr.h"
#include "CScript.h"

#include "CComputeShader.h"
#include "CResMgr.h"

CUserClassMgr::CUserClassMgr()
{
}

CUserClassMgr::~CUserClassMgr()
{
}

CScript* CUserClassMgr::GetNewScript(const std::string_view _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);
	if (iter == m_umapScript.end())
		return nullptr;

	CScript* pScript = iter->second(_strKey);
	return pScript;
}

Ptr<CComputeShader> CUserClassMgr::GetNewCS(const std::string_view _strKey)
{
	//이미 등록되어 있는지 확인
	Ptr<CComputeShader> pCS = CResMgr::GetInst()->FindRes<CComputeShader>(_strKey);
	if (nullptr != pCS)
		return pCS;

	//없을 경우 직접 찾아서 반환
	const auto& iter = m_umapCS.find(_strKey);

	if (iter == m_umapCS.end())
		return nullptr;

	Ptr<CComputeShader> NewCS = iter->second();
	NewCS->SetKey(_strKey);

	//리소스 매니저에 리소스를 추가한다.
	CResMgr::GetInst()->AddRes(_strKey, NewCS);
	
	return NewCS;
}





