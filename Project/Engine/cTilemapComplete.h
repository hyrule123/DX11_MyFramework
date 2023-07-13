#pragma once
#include "cTilemap.h"

class cTilemapComplete :
    public cTilemap
{
public:
    cTilemapComplete();
    virtual ~cTilemapComplete();
    
    cTilemapComplete(cTilemapComplete const& _other) = default;
    CLONE(cTilemapComplete);

public:
    //virtual void init() override;
    virtual void finaltick() override {};
    virtual void cleanup() override {}
};

