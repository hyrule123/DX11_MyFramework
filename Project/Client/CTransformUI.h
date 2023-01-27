#pragma once
#include "CComponentUI.h"


class CTransformUI :
    public CComponentUI
{
public:
    CTransformUI();
    virtual ~CTransformUI();

public:
    virtual int render_update() override;
};

