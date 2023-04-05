#include "pch.h"
#include "CScriptHolder.h"

#include "CScript.h"

#include "CCollider.h"

#include "strKeyDefault.h"
#include "jsoncpp.h"

#include "CScriptMgr.h"

#include "CFState.h"
#include "CFStateMgr.h"

CScriptHolder::CScriptHolder()
	: CComponent(eCOMPONENT_TYPE::SCRIPT_HOLDER)
	, m_pFStateMgr()
{
}

CScriptHolder::CScriptHolder(const CScriptHolder& _other)
	: CComponent(_other)
	, m_pFStateMgr()
{
	size_t size = _other.m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddScript(_other.m_vecScript[i]->Clone());
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

	string strKey = string(RES_INFO::PREFAB::COMPONENT::SCRIPT_HOLDER::JSON_KEY::m_vecScript_strKey);
	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		Json::Value ScriptVal;
		
		jVal[strKey].append(m_vecScript[i]->GetName());

		if (false == m_vecScript[i]->SaveJson(&ScriptVal))
			return false;
	}

	return true;
}

bool CScriptHolder::LoadJson(Json::Value* _jVal)
{
	if (nullptr == _jVal)
		return false;
	else if (false == CComponent::LoadJson(_jVal))
		return false;

	Json::Value& jVal = *_jVal;
	
	{
		string strKey = string(RES_INFO::PREFAB::COMPONENT::SCRIPT_HOLDER::JSON_KEY::m_vecScript_strKey);
		if (jVal.isMember(strKey))
		{
			Json::Value& arr = jVal[strKey];

			int arrsize = (int)arr.size();
			for (int i = 0; i < arrsize; ++i)
			{
				if (arr[i].isNull())
					break;

				CScript* newScript = CScriptMgr::GetInst()->GetNewScript(arr[i].asString());

				if (nullptr == newScript)
				{
					ERROR_MESSAGE("Failed to load Script!!");
					return false;
				}

				AddScript(newScript);
			}
		}
	}
	
	//TODO : 
	//jVal[strKey] = Json::Value(Json::ValueType::arrayValue);

	//for (size_t i = 0; i < m_vecScript.size(); ++i)
	//{
	//	jVal[strKey].append(m_vecScript[i]->GetName());
	//}

	return true;
}


bool CScriptHolder::AddScript(CScript* _pScript)
{
	if (nullptr == _pScript)
		return false;

	std::type_index type = _pScript->GetTypeIndex();

	//스크립트 중복 방지
	if (m_umapScript.find(type) != m_umapScript.end())
	{
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

	_pScript->SetHolder(this);
	if (true == GetOwner()->IsInitialized())
		_pScript->init();

	return true;
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

CFState* CScriptHolder::Transition(UINT _eState)
{
	if (m_pFStateMgr)
		return m_pFStateMgr->Transition(_eState);

	return nullptr;
}



void CScriptHolder::tick()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->tick();
	}
}



void CScriptHolder::BeginColiision(CCollider* _Other, const Vec3& _v3HitPoint)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->BeginCollision(_Other, _v3HitPoint);
	}
}

void CScriptHolder::OnCollision(CCollider* _Other, const Vec3& _v3HitPoint)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->OnCollision(_Other, _v3HitPoint);
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
