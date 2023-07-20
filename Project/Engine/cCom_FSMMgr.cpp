#include "pch.h"
#include "cCom_FSMMgr.h"

#include "cGameObject.h"

#include "I_FSM.h"

cCom_FSMMgr::cCom_FSMMgr()
	: m_vecFSM()
	, m_pCurrentFSM()
	, m_uReservedFSM()
{
}

cCom_FSMMgr::cCom_FSMMgr(const cCom_FSMMgr& _other)
	: IScript(_other)
	, m_pCurrentFSM()
{
	for (size_t i = 0; i < m_vecFSM.size(); ++i)
	{
		if (_other.m_vecFSM[i])
		{
			I_FSM* CloneFSM = _other.m_vecFSM[i]->Clone();
			if (false == AddFSM(CloneFSM))
			{
				SAFE_DELETE(CloneFSM);
				assert(false);
			}
			if (_other.m_vecFSM[i] == _other.m_pCurrentFSM)
			{
				m_pCurrentFSM = m_vecFSM[i];
			}
		}
	}
}

cCom_FSMMgr::~cCom_FSMMgr()
{
	size_t size = m_vecFSM.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecFSM[i]);
	}
}

void cCom_FSMMgr::Init()
{
	for (size_t i = 0; i < m_vecFSM.size(); ++i)
	{
		m_vecFSM[i]->Init();
	}
}

void cCom_FSMMgr::Start()
{
}

void cCom_FSMMgr::Tick()
{
}

eFSM_RESULT cCom_FSMMgr::Transition(const tFSM_Event& _tEvent)
{
	return eFSM_RESULT();
}

eFSM_RESULT cCom_FSMMgr::ForceTransition(const tFSM_Event& _tEvent)
{
	return eFSM_RESULT();
}


void cCom_FSMMgr::BeginColiision(ICollider* _Other, const Vec3& _v3HitPoint)
{
	size_t size = m_vecFSM.size();
	for (size_t i = 0; i < size; ++i)
	{
		if(m_vecFSM[i])
			m_vecFSM[i]->BeginColiision(_Other, _v3HitPoint);
	}
}

void cCom_FSMMgr::OnCollision(ICollider* _Other, const Vec3& _v3HitPoint)
{
	size_t size = m_vecFSM.size();
	for (size_t i = 0; i < size; ++i)
	{
		if(m_vecFSM[i])
			m_vecFSM[i]->OnCollision(_Other, _v3HitPoint);
	}
}

void cCom_FSMMgr::EndCollision(ICollider* _Other)
{
	size_t size = m_vecFSM.size();
	for (size_t i = 0; i < size; ++i)
	{
		if(m_vecFSM[i])
			m_vecFSM[i]->EndCollision(_Other);
	}
}

void cCom_FSMMgr::ChangeFSM(const tFSM_Event& _tEvent)
{
}

bool cCom_FSMMgr::AddFSM(I_FSM* _pFSM)
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

	if (GetOwner()->IsStarted())

		return true;
}
