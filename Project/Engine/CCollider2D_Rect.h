#pragma once
#include "CCollider2D.h"
class CCollider2D_Rect :
    public CCollider2D
{
public:
    CCollider2D_Rect();
    CCollider2D_Rect(eCOLLIDER_TYPE _Type);
    CCollider2D_Rect(const CCollider2D_Rect& _other);
    virtual ~CCollider2D_Rect();
    CLONE(CCollider2D_Rect)

private:
    tRectInfo m_tRectInfo;

public:
     const tRectInfo& GetColliderInfo() const { return m_tRectInfo; }

public:
    virtual void UpdateCollider() override;
    virtual void UpdateAABBinfo() override;
    virtual void UpdateSpatialPartitionInfo(vector<Vec2>& _vecSpatialPartitonVtx) override;
    virtual void DebugRender() override;


};

