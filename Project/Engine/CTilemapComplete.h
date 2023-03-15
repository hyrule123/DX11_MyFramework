#pragma once
#include "CTilemap.h"

#include "SCMapDefine.h"

class CTilemapComplete :
    public CTilemap
{
public:
    CTilemapComplete();
    virtual ~CTilemapComplete();
    CLONE(CTilemapComplete);

public:
    virtual void init() override {};
    virtual void finaltick() override {};
    virtual bool render() override;
    virtual void cleanup() override {};

private:
    tMapData m_tMapData;
    bool    m_bMapLoaded;

public:
    bool LoadMap(const string& _strMapName);
};

