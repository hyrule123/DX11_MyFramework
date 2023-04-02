#include "pch.h"
#include "CFStateMgr.h"
#include "CFState.h"

#include "CScriptHolder.h"

CFStateMgr::CFStateMgr(std::type_index _typeIdx, UINT _eNumState)
	: CScript(_typeIdx)
	, m_eNumState(_eNumState)
	, m_vec_pFSM(_eNumState)
	, m_eCurState()
	, m_bBegin()
{
	m_vec_pFSM.resize(m_eNumState);
}

CFStateMgr::CFStateMgr(const CFStateMgr& _other)
	: CScript(_other)
	, m_eNumState(_other.m_eNumState)
	, m_vec_pFSM(_other.m_eNumState)
	, m_eCurState(_other.m_eCurState)
	, m_bBegin()
{

	for (UINT i = 0u; i < m_eNumState; ++i)
	{
		if (nullptr != _other.m_vec_pFSM[i])
		{
			AddFState(i, (CFState*)_other.m_vec_pFSM[i]->Clone());
		}
	}
}

CFStateMgr::~CFStateMgr()
{
	if (nullptr == GetHolder())
	{
		for (size_t i = 0u; i < m_vec_pFSM.size(); ++i)
		{
			if (m_vec_pFSM[i])
				DESTRUCTOR_DELETE(m_vec_pFSM[i]);
		}
	}
}


void CFStateMgr::init()
{
	GetHolder()->SetFStateMgr(this);

	//무조건 idle 상태는 존재해야 한다고 가정함
	assert(nullptr != m_vec_pFSM[0u]);

	for (UINT i = 0; i < m_eNumState; ++i)
	{
		if (m_vec_pFSM[i])
		{
			//Holder에 스크립트 삽입 요청. 내부에서 중복 스크립트는 알아서 추가하지 않으므로
			//이미 스크립트가 추가되어 있어도 문제가 발생하지 않음.
			GetHolder()->AddScript(m_vec_pFSM[i]);
		}
	}

	initFSM();
	Transition(0u);
}

void CFStateMgr::tick()
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

CFState* CFStateMgr::Transition(UINT _eState)
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

void CFStateMgr::AddFState(UINT _uIdx, CFState* _pFState)
{
	assert(m_eNumState > _uIdx && nullptr != _pFState);
	m_vec_pFSM[_uIdx] = _pFState;
	_pFState->SetFStateMgr(this);
}

void CFStateMgr::SwitchState(UINT _eState)
{
	if (m_vec_pFSM[m_eCurState])
		m_vec_pFSM[m_eCurState]->EndState();

	m_eCurState = _eState;

	if (m_vec_pFSM[m_eCurState])
		m_vec_pFSM[m_eCurState]->EnterState();
}
