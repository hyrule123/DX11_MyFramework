#pragma once
#include "IScript.h"

#include "struct.h"



//한 게임오브젝트에서 활용하는 스크립트들 모음
//I_FSM도 여기에서 담당
class I_FSM;
class cCom_FSMMgr
    : public IScript
{
public:
    cCom_FSMMgr();

    cCom_FSMMgr(const cCom_FSMMgr& _other);
    CLONE(cCom_FSMMgr);

    virtual ~cCom_FSMMgr();


public:
    virtual void Init() final;
    virtual void Start() final;
    virtual void Tick() final;
    virtual void FinalTick() final {}
    virtual void CleanUp() final {}

    eFSM_RESULT Transition(const tFSM_Event& _tEvent);
    eFSM_RESULT Transition(UINT _uStateID) { return Transition(tFSM_Event{ _uStateID, }); }
    eFSM_RESULT ForceTransition(const tFSM_Event& _tEvent);

private:
    //Index = I_FSM의 인덱스 번호
    vector<I_FSM*> m_vecFSM;
    I_FSM* m_pCurrentFSM;

    tFSM_Event m_uReservedFSM;
private:
    bool CheckFSMValid(UINT _uStateID) const;
    void ResetReservedFSM() { m_uReservedFSM = tFSM_Event{}; }
    void ChangeFSM(const tFSM_Event& _tEvent);

    //이건 직접 호출할 필요 없음.(AddScript 할 시 알아서 호출 됨)
    bool AddFSM(I_FSM* _pFSM);
    I_FSM* GetCurFSM() const { return m_pCurrentFSM; }
    I_FSM* GetFSM(UINT _uStateID) const { if (CheckFSMValid(_uStateID)) return m_vecFSM[_uStateID]; return nullptr; }

public:
    void BeginColiision(ICollider* _Other, const Vec3& _v3HitPoint);
    void OnCollision(ICollider* _Other, const Vec3& _v3HitPoint);
    void EndCollision(ICollider* _Other);
};

inline bool cCom_FSMMgr::CheckFSMValid(UINT _uStateID) const
{
    if (_uStateID >= m_vecFSM.size())
        return false;
    else if (nullptr == m_vecFSM[_uStateID])
        return false;

    return true;
}





