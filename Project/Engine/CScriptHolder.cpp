#include "pch.h"
#include "CScriptHolder.h"

#include "CScript.h"

#include "CCollider.h"

#include "strKey_Default.h"
#include "jsoncpp.h"

#include "CScriptMgr.h"

#include "CFSM.h"

CScriptHolder::CScriptHolder()
	: CComponent(eCOMPONENT_TYPE::SCRIPT_HOLDER)
	, m_pCurrentFSM()
{
}

CScriptHolder::CScriptHolder(const CScriptHolder& _other)
	: CComponent(_other)
	, m_pCurrentFSM()
	//, m_mapScript()
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
	//m_mapScript.insert(std::make_pair(_pScript->GetName(), _pScript));

	_pScript->SetHolder(this);

	return true;
}

//CScript* CScriptHolder::FindScript(const string& _strKey)
//{
//	size_t size = m_vecScript.size();
//	for (size_t i = 0; i < size; ++i)
//	{
//		if (_strKey == m_vecScript[i]->GetName())
//			return m_vecScript[i];
//	}
//
//	return nullptr;
//}

CScript* CScriptHolder::FindScript(const string& _strViewKey)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_strViewKey == m_vecScript[i]->GetKey())
			return m_vecScript[i];
	}

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

void CScriptHolder::start()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->start();
	}

	//FSM이 등록되어 있을 경우 0번 FSM을 시작 FSM으로 등록
	if (false == m_vecFSM.empty() && nullptr != m_vecFSM[0])
		m_pCurrentFSM = m_vecFSM[0];
	
	m_pCurrentFSM->EnterState();
}

bool CScriptHolder::Transition(UINT _eStateID, tEvent _tEventMsg)
{
	//예외 처리
	if (false == CheckFSMValid(_eStateID))
		return false;

	bool bCheck = false;

	if (
		//현재 State가 존재하지 않으면 바로 검사 통과
		nullptr == m_pCurrentFSM
		||
		//State가 있을 경우 조건 검사
		m_pCurrentFSM->CheckCondition(_eStateID, _tEventMsg)
		)
	{
		//검사 통과 시 state 교체
		if (m_pCurrentFSM)
			m_pCurrentFSM->EndState();

		m_pCurrentFSM = m_vecFSM[_eStateID];
		m_pCurrentFSM->EnterState();
		return true;
	}

	return false;
}



void CScriptHolder::tick()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->tick();
	}

	if(m_pCurrentFSM) m_pCurrentFSM->OnState();
}

bool CScriptHolder::AddFSM(CFSM* _pFSM)
{
	if (nullptr == _pFSM)
		return false;

	UINT stateID = _pFSM->GetStateID();
	
	//state ID가 vector의 사이즈보다 클 경우 공간 확보
	if (stateID < m_vecFSM.size())
	{
		//이미 해당 state가 있을 경우 false 리턴
		if (nullptr != m_vecFSM[stateID])
			return false;
	}
	else
		m_vecFSM.resize(stateID + 1);

	m_vecFSM[stateID] = _pFSM;
	return true;
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
