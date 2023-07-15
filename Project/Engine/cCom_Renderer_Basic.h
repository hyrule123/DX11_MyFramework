#pragma once
#include "IRenderer.h"

#include "Ptr.h"

class cCom_Renderer_Basic :
    public IRenderer
{
    CLONE(cCom_Renderer_Basic);
public:
    cCom_Renderer_Basic();
    //복사할 멤버변수가 없으므로 복사생성 필요 없음.
    ~cCom_Renderer_Basic();

public:

public:
    virtual void finaltick() override;
    virtual bool render() override;

    virtual void cleanup() override {}

private:

public:
    



};

