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
    Vec2 m_vPos;


public:
    virtual bool CheckCollisionRect(CCollider2D_Rect* _other) override;
    virtual bool CheckCollisionCircle(CCollider2D_Circle* _other) override;
    virtual bool CheckCollisionOBB2D(CCollider2D_OBB* _other) override;

    //점-점은 무조건 실패
    virtual bool CheckCollisionPoint(CCollider2D_Point* _other) override { return false; }

public:
    virtual void UpdateColliderInfo() override;
    virtual void DebugRender() override;

};

