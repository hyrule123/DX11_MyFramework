#pragma once
#include "CComponentUI.h"
class CAnimator2DUI :
    public CComponentUI
{
public:
    CAnimator2DUI();
    virtual ~CAnimator2DUI();

public:
    virtual int render_update() override;
};

