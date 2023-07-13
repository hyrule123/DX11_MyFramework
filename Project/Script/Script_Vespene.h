#pragma once
#include "CScript_SCEntity.h"
class CScript_Vespene :
    public CScript_SCEntity
{
public:
    CScript_Vespene(const string_view _strKey);

    CScript_Vespene(const CScript_Vespene& _other) = default;
    CLONE(CScript_Vespene);

    virtual ~CScript_Vespene();


public:
    virtual void init() override;
};

