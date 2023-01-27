#pragma once

#include "CComponentUI.h"

class CScriptHolderUI :
    public CComponentUI
{
public:
    CScriptHolderUI();
    virtual ~CScriptHolderUI();

public:
    virtual int render_update() override;
};