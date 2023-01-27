#pragma once
#include "CComponentUI.h"
class CMeshRenderUI :
    public CComponentUI
{
public:
    CMeshRenderUI();
    virtual ~CMeshRenderUI();

public:
    virtual int render_update() override;
};