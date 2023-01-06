#pragma once
#include "CScript.h"



class CPlayerScript :
    public CScript
{
private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;

public:
    virtual void init() override;
    virtual void tick() override;


    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

