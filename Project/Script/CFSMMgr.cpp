#include "pch.h"
#include "CFSMMgr.h"

#include "CFSM.h"

CFSMMgr::CFSMMgr(std::type_index _typeIdx, UINT _eNumState)
	: CScript(_typeIdx)
	, m_eNumState(_eNumState)
	, m_arr_pFSM()
	, m_eCurState()
	, m_ePrevState(UINT_MAX)
	, m_bBegin()
{
	m_arr_pFSM = new CFSM*[m_eNumState];
	memset(m_arr_pFSM, 0, sizeof(CFSM*) * m_eNumState);
}

CFSMMgr::CFSMMgr(const CFSMMgr& _other)
	: CScript(_other)
	, m_eNumState(_other.m_eNumState)
	, m_arr_pFSM()
	, m_eCurState(_other.m_eCurState)
	, m_ePrevState(_other.m_ePrevState)
	, m_bBegin()
{
	m_arr_pFSM = new CFSM * [m_eNumState];
	memset(m_arr_pFSM, 0, sizeof(CFSM*) * m_eNumState);

	for (UINT i = 0u; i < m_eNumState; ++i)
	{
		if (nullptr != _other.m_arr_pFSM[i])
		{
			m_arr_pFSM[i] = static_cast<CFSM*>(_other.m_arr_pFSM[i]->Clone());
		}
	}

}

CFSMMgr::~CFSMMgr()
{
	//받은 주소는 스크립트 주소이다. -> CScriptHolder에서 삭제되므로 여기서 삭제해 줄 필요가 없다.
	//대신 동적할당한 배열은 제거해 주어야 한다.
	DESTRUCTOR_DELETE_ARRAY(m_arr_pFSM);
}


void CFSMMgr::tick()
{
	if (false == m_bBegin)
	{
		if (m_arr_pFSM[m_eCurState])
			m_arr_pFSM[m_eCurState]->EnterState();

		m_bBegin = true;
	}

	if (nullptr != m_arr_pFSM[m_eCurState])
	{
		m_arr_pFSM[m_eCurState]->OnState();
	}

	//Prev State 갱신
	m_ePrevState = m_eCurState;
}

void CFSMMgr::Transition(UINT _eState)
{
	assert(m_eNumState > _eState);

	//동일한 State일 경우 return
	if (m_eCurState == _eState || nullptr == m_arr_pFSM[_eState])
		return;

	//현재 FSM이 없거나
	//현재 진행 중인 State에 지정한 State로의 변경이 가능한 지 물어보고, true가 반환되면 변경 
	else if (nullptr == m_arr_pFSM[m_eCurState] || true == m_arr_pFSM[m_eCurState]->CheckCondition(_eState))
	{
		SwitchState(_eState);
	}
}

void CFSMMgr::AddFSM(UINT _uIdx, CFSM* _pFSM)
{
	assert(m_eNumState > _uIdx && nullptr != _pFSM);
	m_arr_pFSM[_uIdx] = _pFSM;
	_pFSM->SetFSMMgr(this);
}

void CFSMMgr::SwitchState(UINT _eState)
{
	if (m_arr_pFSM[m_eCurState])
		m_arr_pFSM[m_eCurState]->EndState();

	m_eCurState = _eState;

	if (m_arr_pFSM[m_eCurState])
		m_arr_pFSM[m_eCurState]->EnterState();


}
