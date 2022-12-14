#pragma once
#include "CScript.h"

#define COLOR_KEY VEC4_0

class CPlayerScript :
    public CScript
{
private:
    Vec4 m_ColorKey;

public:
    virtual void tick() override;


    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

