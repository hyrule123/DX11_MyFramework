#pragma once

#include <Engine/cCom_Renderer_TilemapComplete.h>

#include "define_SCMap.h"

enum class eTILEMAP_DEBUGMODE
{
    NONE,
    MEGATILE,
    MINITILE
};

class cCom_Renderer_TilemapSC :
    public cCom_Renderer_TilemapComplete
{
public:
    cCom_Renderer_TilemapSC();
    virtual ~cCom_Renderer_TilemapSC();
    CLONE(cCom_Renderer_TilemapSC);

public:
    virtual void Init() override;
    virtual void Tick() override;

    virtual eRENDER_RESULT Render() override;

private:
    void BindData();
    void UnBind();

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

