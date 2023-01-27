#pragma once
#include "CComponentUI.h"
class CTilemapUI :
    public CComponentUI
{
public:
    CTilemapUI();
    virtual ~CTilemapUI();

public:
    virtual int render_update() override;
};