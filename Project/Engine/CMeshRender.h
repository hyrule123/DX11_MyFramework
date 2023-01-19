#pragma once
#include "CRenderComponent.h"

#include "ptr.h"

class CMeshRender :
    public CRenderComponent
{
private:



public:
    virtual void finaltick() override;
    virtual void render() override;

    virtual void cleanup() override {}

    CLONE(CMeshRender);
public:
    CMeshRender();
    //복사할 멤버변수가 없으므로 복사생성 필요 없음.
    ~CMeshRender();
};

