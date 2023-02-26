#pragma once

#include <Engine/CScript.h>

class CScript_Bullet :
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


    CLONE(CScript_Bullet);
public:
    CScript_Bullet();
    ~CScript_Bullet();
};
