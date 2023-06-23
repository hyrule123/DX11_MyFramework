#pragma once
#include <Engine/CScript.h>
#include <Engine/CTileMap_SC.h>

class CScript_TilemapUnitLoader :
    public CScript
{
public:
    CScript_TilemapUnitLoader(const string_view _strKey);

    CScript_TilemapUnitLoader(const CScript_TilemapUnitLoader& _other) = default;
    CLONE(CScript_TilemapUnitLoader);

    virtual ~CScript_TilemapUnitLoader();

public:
    virtual void init() override;

private:
    vector<Vec2> m_vecStartLocation;

private:
    void LoadUnit(const shared_ptr<SC_Map::tMapData> _pMapData);
    void UnloadUnit();

    void StartLocation();
};

