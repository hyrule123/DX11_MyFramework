#pragma once
#include "ITilemapBase.h"

class cCom_Renderer_TilemapComplete :
    public ITilemapBase
{
public:
    cCom_Renderer_TilemapComplete();
    virtual ~cCom_Renderer_TilemapComplete();
    
    cCom_Renderer_TilemapComplete(cCom_Renderer_TilemapComplete const& _other) = default;
    CLONE(cCom_Renderer_TilemapComplete);

public:
    //virtual void init() override;
    virtual void finaltick() override {};
    virtual void cleanup() override {}
};

