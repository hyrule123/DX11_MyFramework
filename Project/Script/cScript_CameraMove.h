#pragma once

#include <Engine/IScript.h>



class cScript_CameraMove :
    public IScript
{
public:
    cScript_CameraMove(const string_view _strKey);
    virtual ~cScript_CameraMove();

    CLONE(cScript_CameraMove);

private:
    float m_CamSpeed;
    float m_TurningForceRad;

protected:
    void Camera2DMove();
    void Camera3DMove();

public:
    virtual void tick() override;


};

