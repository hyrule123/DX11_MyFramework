#pragma once
#include "CUI_Component.h"


class CUI_Transform :
    public CUI_Component
{
public:
    CUI_Transform();
    virtual ~CUI_Transform();

public:
    virtual int render_update() override;
};

