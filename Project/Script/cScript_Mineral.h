
#pragma once
#include "cScript_SCEntity.h"

#include "S_H_SCUnitGround.hlsli"

class cScript_Mineral :
    public cScript_SCEntity
{
public:
    cScript_Mineral(const string_view _strKey);

    cScript_Mineral(const cScript_Mineral& _other) = default;
    CLONE(cScript_Mineral);

    virtual ~cScript_Mineral();


public:
    virtual void init() override;
    virtual void tick() override;

private:
    eMINERAL_ATLAS_TYPE m_eMineralAtlasType;
public:
    void SetMineralAtlasType(eMINERAL_ATLAS_TYPE _eType) { m_eMineralAtlasType = _eType; }

private:
    int m_iMineralLeft;
    GETTER_SETTER(int, m_iMineralLeft, MineralLeft);
};

