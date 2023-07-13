#pragma once
#include "cCollider2D.h"


class cCollider2D_OBB :
    public cCollider2D
{
public:
    cCollider2D_OBB();
    cCollider2D_OBB(const cCollider2D_OBB& _other);
    virtual ~cCollider2D_OBB();
    CLONE(cCollider2D_OBB)

private:
    //방향
    tOBB2D m_tOBBInfo;

public:
    const tOBB2D& GetOBBInfo() const { return m_tOBBInfo; }

public:
    virtual void UpdatecCollider() override;
    virtual void UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;
};

