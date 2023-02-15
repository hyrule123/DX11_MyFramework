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
    tOBB2D m_tOBBInfo;

public:
    tOBB2D GetColliderInfo() const { return m_tOBBInfo; }

public:
    virtual void UpdateCollider() override;
    virtual void DebugRender() override;
};

