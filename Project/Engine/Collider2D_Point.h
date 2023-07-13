#pragma once

#include "Collider2D.h"

class Collider2D_Point :
    public Collider2D
{
public:
    Collider2D_Point();
    virtual ~Collider2D_Point();
    CLONE(Collider2D_Point)

private:
    //필요한 변수 없음.(Collider2D::m_v2CenterPos 사용하면 됨)

public:
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

};

