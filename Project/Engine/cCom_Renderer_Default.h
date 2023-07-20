#pragma once
#include "IRenderer.h"

#include "Ptr.h"

class cCom_Renderer_Default :
    public IRenderer
{
    
public:
    cCom_Renderer_Default();
    //복사할 멤버변수가 없으므로 복사생성 필요 없음.
    virtual ~cCom_Renderer_Default();

    cCom_Renderer_Default(cCom_Renderer_Default const& _other) = default;
    CLONE(cCom_Renderer_Default);


public:
    virtual void FinalTick() override;
    virtual eRENDER_RESULT Render() override;

    virtual void CleanUp() override {}

};

