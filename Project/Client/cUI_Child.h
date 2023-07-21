#pragma once
#include "cUI.h"
class cUI_Child :
    public cUI
{
public:
    cUI_Child() = delete;
    cUI_Child(const string& _Name);
    virtual ~cUI_Child();
};

