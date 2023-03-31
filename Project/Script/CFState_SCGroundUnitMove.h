#pragma once
#include <Engine/CFState.h>

#include "defineFSM_SCGroundUnit.h"

class CFState_SCGroundUnitMove :
    public CFState
{
public:
    CFState_SCGroundUnitMove();
    virtual ~CFState_SCGroundUnitMove();

    CFState_SCGroundUnitMove(const CFState_SCGroundUnitMove& _other) = default;
    CLONE(CFState_SCGroundUnitMove);

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

