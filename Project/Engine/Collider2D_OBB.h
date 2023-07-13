#pragma once
#include "Collider2D.h"


class Collider2D_OBB :
    public Collider2D
{
public:
    Collider2D_OBB();
    Collider2D_OBB(const Collider2D_OBB& _other);
    virtual ~Collider2D_OBB();
    CLONE(Collider2D_OBB)

private:
    //방향
    tOBB2D m_tOBBInfo;

public:
    const tOBB2D& GetOBBInfo() const { return m_tOBBInfo; }

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimpleCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;
};

