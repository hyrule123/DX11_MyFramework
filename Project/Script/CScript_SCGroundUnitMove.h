#pragma once
#include "CFSM.h"

#include "SCGroundUnit.h"

class CScript_SCGroundUnitMove :
    public CFSM
{
public:
    CScript_SCGroundUnitMove();
    virtual ~CScript_SCGroundUnitMove();

    CScript_SCGroundUnitMove(const CScript_SCGroundUnitMove& _other) = default;
    CLONE(CScript_SCGroundUnitMove);

public:
    virtual void init() override;
    virtual void tick() override;

    virtual void EnterState() override;
    virtual void OnState() override;
    virtual void EndState() override;

    //상태 변경을 요청한 State의 번호
    //상태 변경이 가능할 경우 true를 반환해 주면 상태를 변경시킬 수 있다.
    virtual bool CheckCondition(UINT _eState) override { return true; }

private:
    float m_fSpeed;
    float m_fTurnSpeed;
    
    bool m_bArrived;
    Vec2 m_v2Destination;

public:
    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetDestination(Vec2 _v2Dest) { m_v2Destination = _v2Dest; m_bArrived = false; }
};

