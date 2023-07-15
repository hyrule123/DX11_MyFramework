#pragma once
#include "IComponent.h"
class ILight :
    public IComponent
{
public:
    ILight(eCOMPONENT_TYPE _type);
    virtual ~ILight();
};

