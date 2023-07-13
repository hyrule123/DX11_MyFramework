#include "pch.h"
#include "ScriptHolder.h"

#include "Script.h"

#include "Collider.h"

#include "strKey_Default.h"
#include "jsoncpp.h"

#include "UserClassMgr.h"

#include "FSM.h"

ScriptHolder::ScriptHolder()
	: Component(eCOMPONENT_TYPE::SCRIPT_HOLDER)
	, m_pCurrentFSM()
{
}

ScriptHolder::ScriptHolder(const ScriptHolder& _other)
	: Component(_other)
	, m_pCurrentFSM()
	//, m_mapScript()
{
	size_t size = _other.m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		Script* pScript = _other.m_vecScript[i]->Clone();
		AddScript(pScript);
	}
}


ScriptHolder::~ScriptHolder()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecScript[i]);
	}
}

bool ScriptHolder::SaveJson(Json::Value* _jVal)
{
	if (nullptr == _jVal)
		return false;
	else if (false == Component::SaveJson(_jVal))
		return false;

	Json::Value& jVal = *_jVal;

	string strKey = string(RES_INFO::PREFAB::COMPONENT::SCRIPT_HOLDER::JSON_KEY::m_vecScript_strKey);
	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		Json::Value ScriptValue = Json::objectValue;

		string strKeyScript(m_vecScript[i]->GetKey());
		ScriptValue[strKeyScript] = Json::Value(Json::objectValue);

		//스크립트도 별도로 저장할 것이 있을경우 별도의 저장 공간을 만들어서 저장시킨다.
		if (false == m_vecScript[i]->SaveJson(&ScriptValue[strKeyScript]))
			return false;

		jVal[strKey].append(ScriptValue);
	}

	return true;
}

bool ScriptHolder::LoadJson(Json::Value* _jVal)
{
	if (nullptr == _jVal)
		return false;
	else if (false == Component::LoadJson(_jVal))
		return false;

	Json::Value& jVal = *_jVal;
	
	try
	{
		string strKey = RES_INFO::PREFAB::COMPONENT::SCRIPT_HOLDER::JSON_KEY::m_vecScript_strKey;
		if (jVal.isMember(strKey))
		{
			Json::Value& arr = jVal[strKey];

			int arrsize = (int)arr.size();
			for (int i = 0; i < arrsize; ++i)
			{
				if (arr[i].empty())
					return false;

				for (Json::ValueIterator it = arr[i].begin(); it != arr[i].end(); ++it)
				{
					string ScriptKey = it.key().asString();

					Script* newScript = UserClassMgr::GetNewScript(ScriptKey);
					
					//스크립트를 생성받지 못했을 경우 return
					if (nullptr == newScript)
					{
						string errmsg = "Script\"";
						errmsg += ScriptKey + "\"";
						errmsg += " is not Exists!!";

						throw(std::runtime_error(errmsg));
					}
					AddScript(newScript);

					//스크립트의 로드에 실패했을 경우
					if (false == newScript->LoadJson(&(*it)[ScriptKey]))
					{
						string errmsg = "Script Load Failed!!";
						throw(errmsg);
					}
				}

			}
		}
	}
	catch (const std::runtime_error& _err)
	{
		ERROR_MESSAGE(_err.what());
		assert(false);
		return false;
	}
	

	return true;
}




bool ScriptHolder::AddScript(Script* _pScript)
{
	if (nullptr == _pScript)
		return false;
	else if (nullptr != FindScript(_pScript->GetName()))
		return false;

	m_vecScript.push_back(_pScript);

	_pScript->SetHolder(this);

	return true;
}


Script* ScriptHolder::FindScript(const string_view _strViewKey)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_strViewKey == m_vecScript[i]->GetKey())
			return m_vecScript[i];
	}

	return nullptr;
}

void ScriptHolder::init()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->init();
	}
}

void ScriptHolder::start()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->start();
	}


	//FSM 등록여부 체크
	if (false == m_vecFSM.empty())
	{
		assert(nullptr != m_vecFSM[0]);

		//FSM이 등록되어 있을 경우 0번 FSM을 시작 FSM으로 등록

		m_pCurrentFSM = m_vecFSM[0];
		m_pCurrentFSM->EnterState(tFSM_Event{ 0u, });
	}
}

eFSM_RESULT ScriptHolder::Transition(const tFSM_Event& _tEvent)
{
	//예외 처리(전환 대상이 되는 상태가 없음)
	if (false == CheckFSMValid(_tEvent.uStateID))
		return eFSM_RESULT::NULLPTR;

	//FSM이 비어있을 경우에는 무조건 성공
	if (nullptr == m_pCurrentFSM)
	{
		ChangeFSM(_tEvent.uStateID);
		return eFSM_RESULT::ACCEPT;
	}

	//현재 State에 상태 변경이 가능한지 조건을 확인
	eFSM_RESULT result = m_pCurrentFSM->CheckCondition(_tEvent);
	if (eFSM_RESULT::ACCEPT == result)
	{
		ChangeFSM(_tEvent.uStateID);
	}
	else if (eFSM_RESULT::RESERVE == result)
	{
		m_uReservedFSM = _tEvent;
	}

	return result;
}

void ScriptHolder::ChangeFSM(const tFSM_Event& _tEvent)
{
	if (m_pCurrentFSM)
		m_pCurrentFSM->EndState();

	m_pCurrentFSM = m_vecFSM[_tEvent.uStateID];
	m_pCurrentFSM->EnterState(_tEvent);

	ResetReservedFSM();
}


eFSM_RESULT ScriptHolder::ForceTransition(const tFSM_Event& _tEvent)
{
	if (false == CheckFSMValid(_tEvent.uStateID))
		return eFSM_RESULT::NULLPTR;

	if (m_pCurrentFSM) m_pCurrentFSM->EndState();
	m_pCurrentFSM = m_vecFSM[_tEvent.uStateID];
	m_pCurrentFSM->EnterState(_tEvent);

	return eFSM_RESULT::ACCEPT;
}



void ScriptHolder::tick()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->tick();
	}

	//예약된 Transition 실행
	if (UINT_MAX != m_uReservedFSM.uStateID)
		//예외처리는 Reserve 등록할떄 했음
		Transition(m_uReservedFSM);

	if(m_pCurrentFSM) m_pCurrentFSM->OnState();
}

void ScriptHolder::BindData()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->BindData();
	}
}

void ScriptHolder::UnBind()
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->UnBind();
	}
}

bool ScriptHolder::AddFSM(FSM* _pFSM)
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

void ScriptHolder::BeginColiision(Collider* _Other, const Vec3& _v3HitPoint)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->BeginCollision(_Other, _v3HitPoint);
	}
}

void ScriptHolder::OnCollision(Collider* _Other, const Vec3& _v3HitPoint)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->OnCollision(_Other, _v3HitPoint);
	}
}

void ScriptHolder::EndCollision(Collider* _Other)
{
	size_t size = m_vecScript.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_vecScript[i]->EndCollision(_Other);
	}
}
