#pragma once
#include "cUI_BasicWindow.h"
class cUIobj_SCPrefabEditor :
    public cUI_BasicWindow
{
public:
    cUIobj_SCPrefabEditor();
    virtual ~cUIobj_SCPrefabEditor();

public:
    virtual void init() override;
    virtual void Tick() override;

    virtual void render_update() override;
};

