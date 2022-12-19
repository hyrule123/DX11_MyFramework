#pragma once
#include "CScript.h"
class CCameraMoveScript :
    public CScript
{
private:
    float m_CamSpeed;
    float m_TurningForceRad;

protected:
    void Camera2DMove();
    void Camera3DMove();

public:
    virtual void tick() override;

public:
    CCameraMoveScript();
    ~CCameraMoveScript();

    CLONE(CCameraMoveScript);
};

