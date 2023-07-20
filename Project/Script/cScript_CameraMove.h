#pragma once

#include <Engine/IScript.h>



class cScript_CameraMove :
    public IScript
{
public:
    cScript_CameraMove();
    virtual ~cScript_CameraMove();

    cScript_CameraMove(const cScript_CameraMove& _other) = default;
    CLONE(cScript_CameraMove);

private:
    float m_CamSpeed;
    float m_TurningForceRad;

protected:
    void Camera2DMove();
    void Camera3DMove();

public:
    virtual void Tick() override;


};

