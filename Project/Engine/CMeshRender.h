#pragma once
#include "CRenderComponent.h"

#include "ptr.h"

class CMeshRender :
    public CRenderComponent
{
    CLONE(CMeshRender);
public:
    CMeshRender();
    //������ ��������� �����Ƿ� ������� �ʿ� ����.
    ~CMeshRender();

public:
    virtual void finaltick() override;
    virtual bool render() override;

    virtual void cleanup() override {}

private:

public:
    



};

