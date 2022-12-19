#pragma once
#include "CComponent.h"
class CCamera :
    public CComponent
{
private:
    float m_AspectRatio;
    ePROJ_TYPE m_ProjectionType;

    Matrix m_matView;
    Matrix m_matProj;

public:
    void SetProjType(ePROJ_TYPE _Type);
    ePROJ_TYPE GetProjType() const { return m_ProjectionType; }

public:
    virtual void init() override;
    virtual void finaltick() override;

public:
    CCamera();
    ~CCamera();

    CLONE(CCamera)
};

