#pragma once
#include "CComponentUI.h"
class CCollider2DUI :
    public CComponentUI
{
public:
    CCollider2DUI();
    virtual ~CCollider2DUI();

public:
    virtual int render_update() override;
};

