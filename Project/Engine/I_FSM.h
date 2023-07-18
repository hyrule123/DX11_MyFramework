#pragma once

#include "IEntity.h"
#include "struct.h"


class cCom_FSMMgr;
class ICollider;

//FSM Manager
class I_FSM
    : public IEntity
{
private:
    I_FSM() = delete;
public:
    //strKey -> IScript로 전달, _uStateID -> I_FSM으로 전달
    I_FSM(UINT _uStateID);

    I_FSM(const I_FSM& _other);
    virtual I_FSM* Clone() = 0;

    virtual ~I_FSM();
    
public:
    virtual void init() {};

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual eFSM_RESULT CheckCondition(const tFSM_Event& _tEvent)= 0;

    //호출 시점: Transition()에서 true가 반환될 경우(상태 진입)
    virtual void EnterState(const tFSM_Event& _tEvent) = 0;

    //호출 시점: 자신이 현재 State일 경우, 스크립트의 tick() 순회 후 호출(상태 진행)
    virtual void OnState() = 0;

    //호출 시점: Transition()에서 true가 반환될 경우(상태 종료)
    virtual void EndState() = 0;

    virtual void BeginColiision(ICollider* _Other, const Vec3& _v3HitPoint) {};
    virtual void OnCollision(ICollider* _Other, const Vec3& _v3HitPoint) {};
    virtual void EndCollision(ICollider* _Other) {};

protected:
    eFSM_RESULT Transition(const tFSM_Event& _tEvent);

private:
    const UINT m_uStateID;
public:
    UINT GetStateID() const { return m_uStateID; }

private:
    cCom_FSMMgr* m_FSMMgr;
public:
    void SetOwner(cCom_FSMMgr* _pMgr) { m_FSMMgr = _pMgr; }
    cCom_FSMMgr* GetFSMMgr() const { return m_FSMMgr; }
};


