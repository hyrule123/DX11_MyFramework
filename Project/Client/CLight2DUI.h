#pragma once
#include "CComponentUI.h"
class CLight2DUI :
    public CComponentUI
{
public:
    CLight2DUI();
    virtual ~CLight2DUI();

public:
    virtual int render_update() override;
};
