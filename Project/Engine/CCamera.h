#pragma once
#include "CComponent.h"
class CCamera :
    public CComponent
{
private:
    float m_AspectRatio;

    Matrix m_matView;
    Matrix m_matProj;

public:
    virtual void init() override;
    virtual void finaltick() override;

    CCamera();
    ~CCamera();

    CLONE(CCamera)
};

