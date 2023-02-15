#pragma once
#include "CCollider2D.h"
class CCollider2D_Rect :
    public CCollider2D
{
public:
    CCollider2D_Rect();
    CCollider2D_Rect(eCOLLIDER_TYPE_2D _Type);
    virtual ~CCollider2D_Rect();
    CLONE(CCollider2D_Rect)

private:
    RectLength m_RectSideInfo;
    tRectInfo  m_tRectInfo;


public:
     const tRectInfo& GetColliderInfo() const { return m_tRectInfo; }

public:
    virtual void UpdateCollider() override;
    virtual void DebugRender() override;


};

