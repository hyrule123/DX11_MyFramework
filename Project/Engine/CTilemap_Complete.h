#pragma once
#include "CTilemap.h"

#include "SCMapDefine.h"

class CTilemap_Complete :
    public CTilemap
{
public:
    CTilemap_Complete();
    virtual ~CTilemap_Complete();
    CLONE(CTilemap_Complete);

public:
    virtual void init() override {};
    virtual void finaltick() override {};
    virtual bool render() override;
    virtual void cleanup() override {};

private:
    tMapData m_tMapData;
    bool    m_bMapLoaded;

public:
    bool LoadMap(const wstring& _strMapName);
};

