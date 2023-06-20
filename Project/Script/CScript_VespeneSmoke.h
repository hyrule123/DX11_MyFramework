#pragma once
#include "CScript_SCEntity.h"

class CScript_VespeneSmoke :
    public CScript_SCEntity
{
public:
    CScript_VespeneSmoke(const string& _strKey);

    CScript_VespeneSmoke(const CScript_VespeneSmoke& _other) = default;
    CLONE(CScript_VespeneSmoke);

    virtual ~CScript_VespeneSmoke();


public:
    virtual void init() override;
    virtual void tick() override;
};

