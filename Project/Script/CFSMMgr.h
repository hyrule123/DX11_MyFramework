#pragma once

#include <Engine/CScript.h>

class CFSM;
class CFSMMgr :
    public CScript
{
public:
    //_eNumState = 등록된 State의 갯수
    CFSMMgr(std::type_index _typeIdx, UINT _eNumState);
    CFSMMgr(const CFSMMgr& _other);
    virtual ~CFSMMgr();
    CLONE(CFSMMgr);

public:
    virtual void tick() final;


    void Transition(UINT _eState);
    

private:
    //포인터를 담아놓을 배열(이중 포인터)
    //사전에 지정된 enum class를 사용하므로 const로 선언
    const UINT m_eNumState;
    CFSM** m_arr_pFSM;

    UINT m_eCurState;
    UINT m_ePrevState;

    bool m_bBegin;
    
public:
    //새 FSM을 등록하고 자신의 인덱스 번호를 반환받는다.
    void AddFSM(UINT _uIdx, CFSM* _pFSM);
    
private:
    void SwitchState(UINT _eState);
};



