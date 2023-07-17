#pragma once

#include <Engine/IScript.h>

#include "define_SCMap.h"

enum class eTILEMAP_DEBUGMODE
{
    NONE,
    MEGATILE,
    MINITILE
};

class cScript_TilemapSC :
    public IScript
{
public:
    cScript_TilemapSC(const string_view _strKey);
    virtual ~cScript_TilemapSC();
    CLONE(cScript_TilemapSC);

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void BindData() override;
    virtual void UnBind() override;

private:
    shared_ptr<SC_Map::tMapData> m_pMapData;
    bool    m_bMapLoaded;
    bool    m_bUnitLoaded;

public:
    bool LoadMap(const string_view _strMapName);


private:
    eTILEMAP_DEBUGMODE m_eDebugMode;
public:
    void SetTilemapDebugMode(eTILEMAP_DEBUGMODE _eMode) { m_eDebugMode = _eMode; }


private:
    vector<Vec2> m_vecStartLocation;

private:
    void LoadUnit();
    void UnloadUnit();

    void StartLocation();
};

