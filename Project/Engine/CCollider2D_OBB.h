#pragma once
#include "CCollider2D.h"

struct tOBB2D
{
    Vec2 m_vAxis[2];
    Vec2 m_vMiddle;
};


class CCollider2D_OBB :
    public CCollider2D
{
public:
    CCollider2D_OBB();
    CCollider2D_OBB(const CCollider2D& _other);
    virtual ~CCollider2D_OBB();
    CLONE(CCollider2D_OBB)

private:
    tOBB2D m_tOBBInfo;

public:
    tOBB2D GetOBBInfo() const { return m_tOBBInfo; }

public:
    virtual bool CheckCollisionRect(CCollider2D_Rect* _other) override;
    virtual bool CheckCollisionCircle(CCollider2D_Circle* _other) override;
    virtual bool CheckCollisionOBB2D(CCollider2D_OBB* _other) override;
    virtual bool CheckCollisionPoint(CCollider2D_Point* _other) override { return false; };

public:
    virtual void UpdateColliderInfo() override;
    virtual void DebugRender() override;
};

