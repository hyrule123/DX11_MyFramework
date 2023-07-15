#pragma once

#include "ICollider2D.h"

class cCom_Coll2D_Point :
    public ICollider2D
{
public:
    cCom_Coll2D_Point();
    virtual ~cCom_Coll2D_Point();
    CLONE(cCom_Coll2D_Point)

private:
    //필요한 변수 없음.(ICollider2D::m_v2CenterPos 사용하면 됨)

public:
    virtual void UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;

};

