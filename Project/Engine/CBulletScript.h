#pragma once
#include "CScript.h"
class CBulletScript :
    public CScript
{
private:
    Vec3 m_vDefaultPos;
    Vec3 m_vBulletDir;
    float m_fBulletSpeed;
    float m_fTimeLeft;

public:
    virtual void init() override;
    virtual void tick() override;

    void SetDefaultVal(const Vec3& _vPos, const Vec3& _vDir);


    CLONE(CBulletScript);
public:
    CBulletScript();
    ~CBulletScript();
};
