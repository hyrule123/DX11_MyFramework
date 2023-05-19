#pragma once

#include "CScript.h"

//UINT 값은 각 FSM 단위별로 enum class를 만들어서 관리할 것



class CFSM;
class CFSM_Mgr :
    public CScript
{
public:
    //_eNumState = 등록된 State의 갯수
    CFSM_Mgr(std::type_index _typeIdx, UINT _eNumState);

    CFSM_Mgr(const CFSM_Mgr& _other);
    CLONE_DISABLE(CFSM_Mgr);

    virtual ~CFSM_Mgr();

public:
    //init에서 반드시 해줘야 하는 고정작업이 있으므로 initFSM()을 재정의해서 초기화 할것
    virtual void init() final;
    virtual void initFSM() {};
    virtual void tick() final;

    CFSM* Transition(UINT _eState);

    //충돌 발생 시 처리는 이 메소드를 오버라이딩해서 사용해주면 된다.
    virtual void BeginCollision(CCollider* _other, const Vec3& _v3HitPoint) override;
    virtual void OnCollision(CCollider* _other, const Vec3& _v3HitPoint) override;
    virtual void EndCollision(CCollider* _other) override;
    
protected:
    //새 FSM을 등록한다. 상속받은 클래스의 생성자에서 호출하는 용도
    //FSM은 정적으로 직접 생성할 것
    void AddFState(UINT _uIdx, CFSM* _pFState);

private:
    void SwitchState(UINT _eState);

private:
    //포인터를 담아놓을 배열(이중 포인터)
    //사전에 지정된 enum class를 사용하므로 const로 선언
    const UINT m_eNumState;
    vector<CFSM*> m_vec_pFSM;

    UINT m_eCurState;

    bool m_bBegin;

public:
    const CFSM* GetFState(UINT _eState) const;
};

inline const CFSM* CFSM_Mgr::GetFState(UINT _eState) const
{
    if (_eState > m_eNumState)
        return nullptr;

    return m_vec_pFSM[_eState];
}

