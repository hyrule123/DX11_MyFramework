#pragma once
#include "IComponent.h"
class IAnimator :
    public IComponent
{
public:
    IAnimator() = default;
    virtual ~IAnimator() = default;

    IAnimator(IAnimator const& _other) = default;
    CLONE_DISABLE(IAnimator);
};

