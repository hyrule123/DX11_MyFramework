#pragma once
#include "CRenderComponent.h"

#include "ptr.h"

class CMeshRender :
    public CRenderComponent
{
private:



public:
    virtual void finaltick() override;
    virtual void render(CCamera* _pCam) override;

    virtual void cleanup() override {}

    CLONE(CMeshRender);
public:
    CMeshRender();
    //������ ��������� �����Ƿ� ������� �ʿ� ����.
    ~CMeshRender();
};

