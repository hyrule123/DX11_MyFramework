#pragma once
#include "CComponent.h"
class CLight :
    public CComponent
{
public:
    CLight(eCOMPONENT_TYPE _type);
    virtual ~CLight();
};

