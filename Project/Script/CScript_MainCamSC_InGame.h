#pragma once

#include <Engine/CScript.h>

class CScript_MainCamSC_InGame :
    public CScript
{
public:
    CScript_MainCamSC_InGame(const string_view _strKey);
    virtual ~CScript_MainCamSC_InGame();

    CScript_MainCamSC_InGame(CScript_MainCamSC_InGame const& _other) = default;
    CLONE(CScript_MainCamSC_InGame);

public:
    void init() override;
    void tick() override;

private:


};

