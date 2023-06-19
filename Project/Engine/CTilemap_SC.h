#pragma once
#include "CTilemap.h"

#include "SCMapDefine.h"

class CTilemap_SC :
    public CTilemap
{
public:
    CTilemap_SC();
    virtual ~CTilemap_SC();
    CLONE(CTilemap_SC);

public:
    virtual void init() override {};
    virtual void tick() override;
    virtual void finaltick() override {};
    virtual bool render() override;
    virtual void cleanup() override {};

private:
    SC_Map::tMapData m_tMapData;
    bool    m_bMapLoaded;
    bool    m_bUnitLoaded;

public:
    bool LoadMap(const string& _strMapName);

private:
    std::function<void(const SC_Map::tMapData&)> m_funcLoadUnit;
public:
    void SetFunc_LoadUnit(std::function<void(const SC_Map::tMapData&)> _pFunc) { m_funcLoadUnit = _pFunc; }
};

