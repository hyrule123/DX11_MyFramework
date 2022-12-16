#pragma once
#include "CScript.h"
class CCameraMoveScript :
    public CScript
{
private:
    float m_CamSpeed;

public:
    virtual void tick() override;

public:
    CCameraMoveScript();
    ~CCameraMoveScript();

    CLONE(CCameraMoveScript);
};

