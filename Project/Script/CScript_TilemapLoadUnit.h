#pragma once
#include <Engine/CScript.h>
#include <Engine/CTileMap_SC.h>

class CScript_TilemapLoadUnit :
    public CScript
{
public:
    CScript_TilemapLoadUnit(const string& _strKey);

    CScript_TilemapLoadUnit(const CScript_TilemapLoadUnit& _other) = default;
    CLONE(CScript_TilemapLoadUnit);

    virtual ~CScript_TilemapLoadUnit();

public:
    virtual void init() override;

private:
    void LoadUnit(const SC_Map::tMapData& _data);
};

