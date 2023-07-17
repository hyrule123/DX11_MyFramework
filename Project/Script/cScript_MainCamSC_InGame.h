#pragma once

#include <Engine/IScript.h>

class cCom_Camera;
class cScript_MainCamSC_InGame :
    public IScript
{
public:
    cScript_MainCamSC_InGame(const string_view _strKey);
    virtual ~cScript_MainCamSC_InGame();

    cScript_MainCamSC_InGame(cScript_MainCamSC_InGame const& _other) = default;
    CLONE(cScript_MainCamSC_InGame);

public:
    void init() override;
    void tick() override;

private:
    void CameraMove();

private:
    DECLARE_GETTER_SETTER(float, m_fCamSpeed, CamSpeed);

private:
    cCom_Camera* m_pCam;

};

