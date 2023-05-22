#include "pch.h"
#include "CScriptHolder.h"

#include "CScript.h"

#include "CCollider.h"

#include "strKeyDefault.h"
#include "jsoncpp.h"

#include "CScriptMgr.h"

#include "CFSM.h"
#include "CFSM_Mgr.h"

CScriptHolder::CScriptHolder()
	: CComponent(eCOMPONENT_TYPE::SCRIPT_HOLDER)
{
}

CScriptHolder::CScriptHolder(const CScriptHolder& _other)
	: CComponent(_other)
	, m_mapScript()
{
	size_t size = _other.m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		CScript* pScript = _other.m_vecScript[i]->Clone();
		AddScript(pScript);
	}
}


CScriptHolder::~CScriptHolder()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecScript[i]);
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
		jVal[strKey].append(m_vecScript[i]->GetName());

		Json::Value ScriptVal;
		//스크립트도 별도로 저장할 것이 있을경우 저장시킨다.
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
	else if (nullptr != FindScript(_pScript->GetName()))
		return false;

	m_vecScript.push_back(_pScript);
	m_mapScript.insert(std::make_pair(_pScript->GetName(), _pScript));

	_pScript->SetHolder(this);

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

void CScriptHolder::start()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->start();
	}
}

CFSM* CScriptHolder::Transition(UINT _eState)
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
