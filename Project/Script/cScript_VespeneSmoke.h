#pragma once
#include "cScript_SCEntity.h"

class cScript_VespeneSmoke :
    public cScript_SCEntity
{
public:
    cScript_VespeneSmoke(const string_view _strKey);

    cScript_VespeneSmoke(const cScript_VespeneSmoke& _other) = default;
    CLONE(cScript_VespeneSmoke);

    virtual ~cScript_VespeneSmoke();

public:
    virtual void init() override;
    virtual void tick() override;

private:
    float m_fRandVal;
};

