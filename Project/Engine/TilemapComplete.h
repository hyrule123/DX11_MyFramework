#pragma once
#include "Tilemap.h"

class TilemapComplete :
    public Tilemap
{
public:
    TilemapComplete();
    virtual ~TilemapComplete();
    
    TilemapComplete(TilemapComplete const& _other) = default;
    CLONE(TilemapComplete);

public:
    //virtual void init() override;
    virtual void finaltick() override {};
    virtual void cleanup() override {}
};

