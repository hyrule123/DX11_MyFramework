#pragma once
#include "CUI_Widget.h"

#include <Engine/ptr.h>

class CTexture;

class CUI_Image :
    public CUI_Widget
{
public:
    CUI_Image();
    virtual ~CUI_Image();

private:
    Ptr<CTexture> m_TargetImage;
    

public:
};

