#include "pch.h"
#include "UserClassMgr.h"
#include "Script.h"

#include "C_ComputeShader.h"
#include "ResMgr.h"

std::unordered_map <std::string_view, std::function<Script* (const string_view)>> UserClassMgr::m_umapScript;
std::unordered_map <std::string_view, std::function<Ptr<C_ComputeShader>()>> UserClassMgr::m_umapCS;
std::unordered_map <std::string_view, std::function<ParticleSystem* ()>>	UserClassMgr::m_umapParticle;

Script* UserClassMgr::GetNewScript(const std::string_view _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);
	if (iter == m_umapScript.end())
		return nullptr;

	Script* pScript = iter->second(_strKey);
	return pScript;
}

Ptr<C_ComputeShader> UserClassMgr::GetNewCS(const std::string_view _strKey)
{
	//이미 등록되어 있는지 확인
	Ptr<C_ComputeShader> pCS = ResMgr::GetInst()->FindRes<C_ComputeShader>(_strKey);
	if (nullptr != pCS)
		return pCS;

	//없을 경우 직접 찾아서 반환
	const auto& iter = m_umapCS.find(_strKey);

	if (iter == m_umapCS.end())
		return nullptr;

	Ptr<C_ComputeShader> NewCS = iter->second();
	NewCS->SetKey(_strKey);

	//리소스 매니저에 리소스를 추가한다.
	ResMgr::GetInst()->AddRes(_strKey, NewCS);

	return NewCS;
}
