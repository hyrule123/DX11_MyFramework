#pragma once

#include <Engine/CScript.h>



class CScript_CameraMove :
    public CScript
{
public:
    CScript_CameraMove(const string& _strKey);
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

