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

Ptr<CComputeShader> CUserClassMgr::GetNewCS(const std::string_view _strKey)
{
	const auto& iter = m_umapCS.find(_strKey);

	if (iter == m_umapCS.end())
		return nullptr;

	Ptr<CComputeShader> NewCS = iter->second();
	NewCS->SetKey(_strKey);

	//리소스 매니저에 리소스를 추가한다.
	CResMgr::GetInst()->AddRes(_strKey, NewCS);
	
	return NewCS;
}





