#pragma once
#include "cCollider3D.h"


class cCollider3D_Ray :
    public cCollider3D
{
public:
    cCollider3D_Ray();
    virtual ~cCollider3D_Ray();


private:
    Vec3 m_vPos;
    Vec3 m_vDir;
    float m_fLength;


public:

public:
    virtual void UpdatecColliderInfo() override;
    virtual void DebugRender() override;


};

