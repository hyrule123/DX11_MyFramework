#pragma once
#include "IRenderer.h"

#include "Ptr.h"

class cCom_Renderer_Basic :
    public IRenderer
{
    
public:
    cCom_Renderer_Basic();
    //복사할 멤버변수가 없으므로 복사생성 필요 없음.
    virtual ~cCom_Renderer_Basic();

    cCom_Renderer_Basic(cCom_Renderer_Basic const& _other) = default;
    CLONE(cCom_Renderer_Basic);

public:

public:
    virtual void FinalTick() override;
    virtual eRENDER_RESULT Render() override;

    virtual void CleanUp() override {}

private:

public:
};

