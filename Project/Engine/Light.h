#pragma once
#include "Component.h"
class Light :
    public Component
{
public:
    Light(eCOMPONENT_TYPE _type);
    virtual ~Light();
};

