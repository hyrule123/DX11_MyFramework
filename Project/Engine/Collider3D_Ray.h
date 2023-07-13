#pragma once
#include "Collider3D.h"


class Collider3D_Ray :
    public Collider3D
{
public:
    Collider3D_Ray();
    virtual ~Collider3D_Ray();


private:
    Vec3 m_vPos;
    Vec3 m_vDir;
    float m_fLength;


public:

public:
    virtual void UpdateColliderInfo() override;
    virtual void DebugRender() override;


};

