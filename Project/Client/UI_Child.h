#pragma once
#include "CUI.h"
class CUI_Child :
    public CUI
{
public:
    CUI_Child() = delete;
    CUI_Child(const string& _Name);
    virtual ~CUI_Child();
};

