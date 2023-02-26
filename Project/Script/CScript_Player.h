#pragma once

#include <Engine/CScript.h>

class CScript_Player :
    public CScript
{
private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;

public:
    virtual void init() override;
    virtual void tick() override;

private:
    void Shoot();


    
public:
    CScript_Player();
    ~CScript_Player();

    CLONE(CScript_Player);
};

