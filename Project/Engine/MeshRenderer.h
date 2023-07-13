#pragma once
#include "RenderComponent.h"

#include "Ptr.h"

class MeshRenderer :
    public RenderComponent
{
    CLONE(MeshRenderer);
public:
    MeshRenderer();
    //복사할 멤버변수가 없으므로 복사생성 필요 없음.
    ~MeshRenderer();

public:

public:
    virtual void finaltick() override;
    virtual bool render() override;

    virtual void cleanup() override {}

private:

public:
    



};

