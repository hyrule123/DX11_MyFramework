#pragma once
#include "ICollider2D.h"


class cCom_Coll2D_OBB :
    public ICollider2D
{
public:
    cCom_Coll2D_OBB();
    cCom_Coll2D_OBB(const cCom_Coll2D_OBB& _other);
    virtual ~cCom_Coll2D_OBB();
    CLONE(cCom_Coll2D_OBB)

private:
    //방향
    tOBB2D m_tOBBInfo;

public:
    const tOBB2D& GetOBBInfo() const { return m_tOBBInfo; }

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSimplecCollider(Vec4& _vSimpleCollLBRTPos) override;
    virtual void DebugRender() override;
};

