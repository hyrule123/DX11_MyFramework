#pragma once

#include "cCollider2D.h"

class cCollider2D_Point :
    public cCollider2D
{
public:
    cCollider2D_Point();
    virtual ~cCollider2D_Point();
    CLONE(cCollider2D_Point)

private:
    //필요한 변수 없음.(cCollider2D::m_v2CenterPos 사용하면 됨)

public:
    virtual void UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

};

