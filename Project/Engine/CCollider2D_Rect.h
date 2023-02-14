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
    tOBB2D m_tOBBInfo;

public:
    tOBB2D GetColliderInfo() const { return m_tOBBInfo; }

public:
    virtual void UpdateColliderInfo() override;
    virtual void DebugRender() override;


};

