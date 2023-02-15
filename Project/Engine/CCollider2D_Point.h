#pragma once

#include "CCollider2D.h"

class CCollider2D_Point :
    public CCollider2D
{
public:
    CCollider2D_Point();
    virtual ~CCollider2D_Point();
    CLONE(CCollider2D_Point)

private:
    //필요한 변수 없음.(CCollider2D::m_vCenterPos 사용하면 됨)

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

};

