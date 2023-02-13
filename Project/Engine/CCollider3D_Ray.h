#pragma once
#include "CCollider3D.h"


class CCollider3D_Ray :
    public CCollider3D
{
public:
    CCollider3D_Ray();
    virtual ~CCollider3D_Ray();


private:
    Vec3 m_vPos;
    Vec3 m_vDir;
    float m_fLength;


public:

public:
    virtual void UpdateColliderInfo() override;
    virtual void DebugRender() override;


};

