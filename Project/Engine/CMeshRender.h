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

    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};

