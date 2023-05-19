#include "pch.h"
#include "CFSM_Mgr.h"
#include "CFSM.h"

#include "CScriptHolder.h"

CFSM_Mgr::CFSM_Mgr(std::type_index _typeIdx, UINT _eNumState)
	: CScript(_typeIdx)
	, m_eNumState(_eNumState)
	, m_vec_pFSM(_eNumState)
	, m_eCurState()
	, m_bBegin()
{
	m_vec_pFSM.resize(m_eNumState);
}

CFSM_Mgr::CFSM_Mgr(const CFSM_Mgr& _other)
	: CScript(_other)
	, m_eNumState(_other.m_eNumState)
	, m_vec_pFSM(_other.m_eNumState)
	, m_eCurState(_other.m_eCurState)
	, m_bBegin()
{
	for (UINT i = 0u; i < m_eNumState; ++i)
	{
		if (_other.m_vec_pFSM[i])
		{
			AddFState(i, (CFSM*)_other.m_vec_pFSM[i]->Clone());
		}
	}
}

CFSM_Mgr::~CFSM_Mgr()
{
	for (size_t i = 0u; i < m_vec_pFSM.size(); ++i)
	{
		if (m_vec_pFSM[i])
			DESTRUCTOR_DELETE(m_vec_pFSM[i]);
	}
}


void CFSM_Mgr::init()
{
	ScriptHolder()->RegisterFStateMgr(this);

	//무조건 idle 상태는 존재해야 한다고 가정함
	assert(0u < (UINT)m_vec_pFSM.size() && nullptr != m_vec_pFSM[0]);

	for (size_t i = 0u; i < m_vec_pFSM.size(); ++i)
	{
		if (m_vec_pFSM[i])
			m_vec_pFSM[i]->init();
	}

	initFSM();
	Transition(0u);
}

void CFSM_Mgr::tick()
{
	if (false == m_bBegin)
	{
		if (m_vec_pFSM[m_eCurState])
			m_vec_pFSM[m_eCurState]->EnterState();

		m_bBegin = true;
	}

	if (nullptr != m_vec_pFSM[m_eCurState])
	{
		m_vec_pFSM[m_eCurState]->OnState();
	}
}

CFSM* CFSM_Mgr::Transition(UINT _eState)
{
	//Array 인덱스보다 높은 숫자가 들어올 경우 assert
	if (m_eNumState < _eState)
		return nullptr;

	//목표 State가 존재하지 않을 경우 return
	if (nullptr == m_vec_pFSM[_eState])
		return nullptr;

	//현재 FSM이 없거나
	//현재 진행 중인 State에 지정한 State로의 변경이 가능한 지 물어보고, true가 반환되면 변경 
	else if (true == m_vec_pFSM[m_eCurState]->CheckCondition(_eState))
	{
		SwitchState(_eState);
		return m_vec_pFSM[m_eCurState];
	}

	return nullptr;
}

void CFSM_Mgr::BeginCollision(CCollider* _other, const Vec3& _v3HitPoint)
{
	for (UINT i = 0u; i < m_eNumState; ++i)
	{
		if (m_vec_pFSM[i])
			m_vec_pFSM[i]->BeginColiision(_other, _v3HitPoint);
	}
}

void CFSM_Mgr::OnCollision(CCollider* _other, const Vec3& _v3HitPoint)
{
	for (UINT i = 0u; i < m_eNumState; ++i)
	{
		if (m_vec_pFSM[i])
			m_vec_pFSM[i]->OnCollision(_other, _v3HitPoint);
	}
}

void CFSM_Mgr::EndCollision(CCollider* _other)
{
	for (UINT i = 0u; i < m_eNumState; ++i)
	{
		if (m_vec_pFSM[i])
			m_vec_pFSM[i]->EndCollision(_other);
	}
}

void CFSM_Mgr::AddFState(UINT _uIdx, CFSM* _pFState)
{
	assert(m_eNumState > _uIdx && nullptr != _pFState);
	m_vec_pFSM[_uIdx] = _pFState;
	_pFState->SetFStateMgr(this);
}

void CFSM_Mgr::SwitchState(UINT _eState)
{
	if (m_vec_pFSM[m_eCurState])
		m_vec_pFSM[m_eCurState]->EndState();

	m_eCurState = _eState;

	if (m_vec_pFSM[m_eCurState])
		m_vec_pFSM[m_eCurState]->EnterState();
}


