#pragma once
#include "CComponentUI.h"
class CCameraUI :
    public CComponentUI
{
public:
    CCameraUI();
    virtual ~CCameraUI();

public:
    virtual int render_update() override;
};