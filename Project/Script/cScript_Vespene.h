#pragma once
#include "cScript_SCEntity.h"
class cScript_Vespene :
    public cScript_SCEntity
{
public:
    cScript_Vespene(const string_view _strKey);

    cScript_Vespene(const cScript_Vespene& _other) = default;
    CLONE(cScript_Vespene);

    virtual ~cScript_Vespene();


public:
    virtual void Init() override;
};

