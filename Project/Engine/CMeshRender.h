#pragma once
#include "CRenderComponent.h"

#include "ptr.h"

class CMeshRender :
    public CRenderComponent
{
private:



public:
    virtual void finaltick() override;
    virtual bool render(int _iCamIdx) override;

    virtual void cleanup() override {}

    CLONE(CMeshRender);
public:
    CMeshRender();
    //������ ��������� �����Ƿ� ������� �ʿ� ����.
    ~CMeshRender();
};

