#include "pch.h"
#include "CScriptHolder.h"

#include "CScript.h"

#include "CCollider.h"

CScriptHolder::CScriptHolder()
	: CComponent(eCOMPONENT_TYPE::SCRIPT_HOLDER)
{
}

CScriptHolder::CScriptHolder(const CScriptHolder& _other)
	: CComponent(_other)
{
	size_t size = _other.m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript.push_back(_other.m_vecScript[i]->Clone());

		//복사 생성자에는 아직 소유자 포인터 정보가 입력되지 않았으므로 
		//SetOwner에서 처리해줘야 한다.
	}
}



CScriptHolder::~CScriptHolder()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		DESTRUCTOR_DELETE(m_vecScript[i]);
	}
}

bool CScriptHolder::SaveJson(Json::Value* _jVal)
{
	if (nullptr == _jVal)
		return false;
	else if (false == CComponent::SaveJson(_jVal))
		return false;

	Json::Value& jVal = *_jVal;

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{



	}

	return false;
}

bool CScriptHolder::LoadJson(Json::Value* _jVal)
{
	return false;
}


bool CScriptHolder::AddScript(CScript* _pScript)
{
	if (nullptr == _pScript)
		return false;

	std::type_index type = _pScript->GetTypeIndex();

	//스크립트 중복 방지
	if (m_umapScript.find(type) != m_umapScript.end())
	{
		delete _pScript;
		return false;
	}
	//size_t size = m_vecScript.size();
	//for (size_t i = 0; i < size; ++i)
	//{
	//	if (type == m_vecScript[i]->GetTypeIndex())
	//	{
	//		delete _pScript;
	//		return false;
	//	}
	//}

	m_vecScript.push_back(_pScript);
	m_umapScript[type] = _pScript;

	_pScript->SetOwner(GetOwner());
	_pScript->SetHolder(this);
	if (true == GetOwner()->IsInitialized())
		_pScript->init();

	return true;
}

CScript* CScriptHolder::GetScript(std::type_index _typeIdx)
{
	const auto& iter = m_umapScript.find(_typeIdx);

	if (iter != m_umapScript.end())
		return iter->second;

	return nullptr;
}

void CScriptHolder::init()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->init();
	}
}

void CScriptHolder::SetOwner(CGameObject* _pOwner)
{
	CComponent::SetOwner(_pOwner);

	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->SetOwner(_pOwner);
	}
}

void CScriptHolder::tick()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->tick();
	}
}



void CScriptHolder::BeginColiision(CCollider* _Other)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->BeginCollision(_Other);
	}
}

void CScriptHolder::OnCollision(CCollider* _Other)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->OnCollision(_Other);
	}
}

void CScriptHolder::EndCollision(CCollider* _Other)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->EndCollision(_Other);
	}
}
