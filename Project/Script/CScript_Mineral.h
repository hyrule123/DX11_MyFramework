
#pragma once
#include "CScript_SCEntity.h"

#include <Engine/S_H_SCUnitGround.hlsli>

class CScript_Mineral :
    public CScript_SCEntity
{
public:
    CScript_Mineral(const string& _strKey);

    CScript_Mineral(const CScript_Mineral& _other) = default;
    CLONE(CScript_Mineral);

    virtual ~CScript_Mineral();


public:
    virtual void init() override;
    virtual void tick() override;

private:
    eMINERAL_ATLAS_TYPE m_eMineralAtlasType;
public:
    void SetMineralAtlasType(eMINERAL_ATLAS_TYPE _eType) { m_eMineralAtlasType = _eType; }

private:
    int m_iMineralMax;
    GETTER_SETTER(int, m_iMineralMax, MineralRemain);
    int m_iMineralsEachLevel;

private:
    int m_iCurMineral;
};

