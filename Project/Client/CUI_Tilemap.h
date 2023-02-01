#pragma once
#include "CUI_Component.h"
class CUI_Tilemap :
    public CUI_Component
{
public:
    CUI_Tilemap();
    virtual ~CUI_Tilemap();

public:
    virtual int render_update() override;
};