#pragma once

#include <Engine/CFState.h>

#include "define_SCUnit.h"

class CFState_SCUnitMove :
    public CFState
{
public:
    CFState_SCUnitMove(std::type_index _typeIdx, UINT _eMyState, eSCUNIT_MOVE_TYPE _eMoveType);
    virtual ~CFState_SCUnitMove();

    CFState_SCUnitMove(const CFState_SCUnitMove& _other) = default;

public:
    virtual void init() override;

private:
    const eSCUNIT_MOVE_TYPE m_eMoveType;

    //이동속도
    float m_fSpeed;

    //선회력
    float m_fTurnSpeed;

    //목적지 관련
    bool m_bArrived;
    Vec2 m_v2Destination;



public:
    GETTER(eSCUNIT_MOVE_TYPE, m_eMoveType, MoveType);
    GETSET(float, m_fSpeed, Speed);

    void SetDestination(Vec2 _v2Dest) { m_v2Destination = _v2Dest; m_bArrived = false; }
    GETTER(Vec2, m_v2Destination, Destination);

    SETTER(bool, m_bArrived, Arrive);
    bool IsArrived() const { return m_bArrived; }
};

