#pragma once
#include "CScript.h"
class CTestObjScript :
    public CScript
{
private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;

public:
    virtual void init() override;
    virtual void tick() override;


    CLONE(CTestObjScript);
public:
    CTestObjScript();
    ~CTestObjScript();
};

