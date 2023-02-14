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
    //�ʿ��� ���� ����.(CCollider2D::m_vCenterPos ����ϸ� ��)

public:
    virtual void UpdateCollider() override;
    virtual void UpdateSpatialPartitionInfo(vector<Vec2>& _vecSpatialPartitonVtx) override;
    virtual void DebugRender() override;

};

