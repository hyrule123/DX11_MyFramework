#pragma once
#include "ICollider3D.h"


class cCom_Coll3D_Ray :
    public ICollider3D
{
public:
    cCom_Coll3D_Ray();
    virtual ~cCom_Coll3D_Ray();


private:
    Vec3 m_vPos;
    Vec3 m_vDir;
    float m_fLength;


public:

public:
    virtual void UpdatecColliderInfo() override;
    virtual void DebugRender() override;


};

