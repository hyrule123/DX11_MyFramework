#pragma once
#include "Entity.h"

class C_CShaderModule :
    public Entity
{
public:
    C_CShaderModule();
    virtual ~C_CShaderModule();

    virtual void BindData() = 0;
    virtual void UnBind() = 0;
};

