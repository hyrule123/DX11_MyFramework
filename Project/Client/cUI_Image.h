#pragma once
#include "cUI_Widget.h"

#include <Engine/ptr.h>

class cTexture;

class CUI_Image :
    public cUI_Widget
{
public:
    CUI_Image();
    virtual ~CUI_Image();

private:
    Ptr<cTexture> m_TargetImage;
    

public:
};

