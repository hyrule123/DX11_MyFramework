
#pragma once
#include "cScript_SCEntity.h"

#include <HLSL/S_H_SCUnitGround.hlsli>

class cScript_Mineral :
    public cScript_SCEntity
{
public:
    cScript_Mineral();

    cScript_Mineral(const cScript_Mineral& _other) = default;
    CLONE(cScript_Mineral);

    virtual ~cScript_Mineral();


public:
    virtual void Init() override;
    virtual void Tick() override;

private:
    eMINERAL_ATLAS_TYPE m_eMineralAtlasType;
public:
    void SetMineralAtlasType(eMINERAL_ATLAS_TYPE _eType) { m_eMineralAtlasType = _eType; }

private:
    int m_iMineralLeft;
    GETTER_SETTER(int, m_iMineralLeft, MineralLeft);
};

