#pragma once

#include <Engine/CScript.h>

class CScript_TestObj :
    public CScript
{
    
public:
    CScript_TestObj(const string_view _strKey);
    virtual ~CScript_TestObj();

    CLONE(CScript_TestObj);

private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;

public:
    virtual void BeginCollision(CCollider* _pCol, const Vec3& _v3HitPoint) override;

public:
    virtual void init() override;
    virtual void tick() override;


};

