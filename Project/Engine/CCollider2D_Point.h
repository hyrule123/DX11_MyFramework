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
    Vec2 m_vWorldPos;

public:
    virtual void UpdateColliderInfo() override;
    virtual void DebugRender() override;

};

