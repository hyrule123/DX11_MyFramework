#pragma once
#include "CScript.h"

class CScript_CameraMove :
    public CScript
{
public:
    CScript_CameraMove();
    virtual ~CScript_CameraMove();

    CLONE(CScript_CameraMove);

private:
    float m_CamSpeed;
    float m_TurningForceRad;

protected:
    void Camera2DMove();
    void Camera3DMove();

public:
    virtual void tick() override;


};

