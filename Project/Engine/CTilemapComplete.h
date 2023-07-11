#pragma once
#include "CTilemap.h"

class CTilemapComplete :
    public CTilemap
{
public:
    CTilemapComplete();
    virtual ~CTilemapComplete();
    
    CTilemapComplete(CTilemapComplete const& _other) = default;
    CLONE(CTilemapComplete);

public:
    //virtual void init() override;
    virtual void finaltick() override {};
    virtual void cleanup() override {}
};

