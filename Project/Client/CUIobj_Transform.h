#pragma once
#include "CUIobj_Component.h"


class CUIobj_Transform :
    public CUIobj_Component
{
public:
    CUIobj_Transform();
    virtual ~CUIobj_Transform();

public:
    virtual void render_update() override;
};

