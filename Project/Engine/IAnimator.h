#pragma once
#include "IComponent.h"
class IAnimator :
    public IComponent
{
public:
    IAnimator();
    virtual ~IAnimator();

    IAnimator(IAnimator const& _other) = default;
    CLONE_DISABLE(IAnimator);
};

