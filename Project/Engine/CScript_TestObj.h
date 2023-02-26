#pragma once
#include "CScript.h"


class CScript_TestObj :
    public CScript
{
private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;

public:
    virtual void BeginCollision(CCollider* _pCol) override;

public:
    virtual void init() override;
    virtual void tick() override;

    CLONE(CScript_TestObj);
public:
    CScript_TestObj();
    ~CScript_TestObj();
};

