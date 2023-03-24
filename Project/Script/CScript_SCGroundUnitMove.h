#pragma once
#include <Engine/CScript.h>


class CScript_SCGroundUnitMove :
    public CScript
{
public:
    CScript_SCGroundUnitMove();
    virtual ~CScript_SCGroundUnitMove();

    CScript_SCGroundUnitMove(const CScript_SCGroundUnitMove& _other) = default;
    CLONE(CScript_SCGroundUnitMove);

public:
    virtual void tick() override;

private:
    float m_fSpeed;
    
    bool m_bArrived;
    Vec2 m_v2Destination;
    

public:
    void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
    void SetDestination(Vec2 _v2Dest) { m_v2Destination = _v2Dest; m_bArrived = false; }
};

