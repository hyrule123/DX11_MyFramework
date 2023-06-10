#pragma once

#include <Engine/CFSM.h>

#include "define_SCUnit.h"

class CFSM_Move_Base :
    public CFSM
{
public:
    CFSM_Move_Base(const string& _strKey, UINT _eMyState, SC::eUNIT_MOVE_TYPE _eMoveType);
    virtual ~CFSM_Move_Base();

    //Base 클래스이므로 disable
    CFSM_Move_Base(const CFSM_Move_Base& _other) = default;
    CLONE_DISABLE(CFSM_Move_Base);

public:
    virtual void init() override;


private:
    const SC::eUNIT_MOVE_TYPE m_eMoveType;
public:
    GETTER(SC::eUNIT_MOVE_TYPE, m_eMoveType, MoveType);

private:
    //이동속도
    DECLARE_GETTER_SETTER(float, m_fSpeed, Speed);

private:
    //초당 선회각
    DECLARE_GETTER_SETTER(float, m_fTurnSpeed, TurnSpeed);

private:
    //목적지 관련
    bool m_bArrived;
public:
    SETTER(bool, m_bArrived, Arrive);
    bool IsArrived() const { return m_bArrived; }

private:
    Vec2 m_v2Destination;
public:
    void SetDestination(Vec2 _v2Dest) { m_v2Destination = _v2Dest; m_bArrived = false; }
    GETTER(Vec2, m_v2Destination, Destination);
};

