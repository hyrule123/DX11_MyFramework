#pragma once
#include "cComponent.h"
class cLight :
    public cComponent
{
public:
    cLight(eCOMPONENT_TYPE _type);
    virtual ~cLight();
};

