#pragma once

#include <Engine/IScript.h>

class cScript_TestObj :
    public IScript
{
    
public:
    cScript_TestObj(const string_view _strKey);
    virtual ~cScript_TestObj();

    CLONE(cScript_TestObj);

private:
    Vec4 m_ColorKey;
    float m_MoveSpeed;
    float m_TurningForceRad;

public:
    virtual void BeginCollision(ICollider* _pCol, const Vec3& _v3HitPoint) override;

public:
    virtual void init() override;
    virtual void tick() override;


};

