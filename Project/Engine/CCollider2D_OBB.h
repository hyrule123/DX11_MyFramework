#pragma once
#include "CCollider2D.h"


class CCollider2D_OBB :
    public CCollider2D
{
public:
    CCollider2D_OBB();
    CCollider2D_OBB(const CCollider2D_OBB& _other);
    virtual ~CCollider2D_OBB();
    CLONE(CCollider2D_OBB)

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

