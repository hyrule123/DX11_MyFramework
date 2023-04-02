#pragma once
#include "CUI_BasicWIndow.h"
class CUIobj_SCPrefabEditor :
    public CUI_BasicWindow
{
public:
    CUIobj_SCPrefabEditor();
    virtual ~CUIobj_SCPrefabEditor();

public:
    virtual void init() override;
    virtual void tick() override;

    virtual void render_update() override;
};

