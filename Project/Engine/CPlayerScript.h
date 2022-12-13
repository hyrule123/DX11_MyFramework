#pragma once
#include "CScript.h"

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

