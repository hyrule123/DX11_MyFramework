#pragma once
#include "CCollider2D.h"


class CCollider2D_Circle :
    public CCollider2D
{
public:
    CCollider2D_Circle();
    CCollider2D_Circle(const CCollider2D_Circle& _other);
    virtual ~CCollider2D_Circle();
    CLONE(CCollider2D_Circle);

private:
    
public:
    virtual void UpdateCollider() override;
    virtual void DebugRender() override;

};

