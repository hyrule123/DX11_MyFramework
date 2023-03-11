#pragma once

#include <Engine/CScript.h>

class CScript_Player :
    public CScript
{
public:
    CScript_Player();
    //CScript_Player(const CScript_Player& _other); //기본 복사생성자로 충분함.
    ~CScript_Player();

    CLONE(CScript_Player);

public:
    virtual void init() override;
    virtual void tick() override;

private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;



private:
    void Shoot();
};

