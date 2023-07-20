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
    //virtual void Init() override;
    //virtual void FinalTick() override;
    //virtual eRENDER_RESULT Render() override;
    virtual void CleanUp() override {}
};

