#pragma once
#include "cScript_SCEntity.h"

class cCom_Animator2D;
class cScript_VespeneSmoke :
    public cScript_SCEntity
{
public:
    cScript_VespeneSmoke();

    cScript_VespeneSmoke(const cScript_VespeneSmoke& _other) = default;
    CLONE(cScript_VespeneSmoke);

    virtual ~cScript_VespeneSmoke();

public:
    virtual void Init() override;
    virtual void Tick() override;

private:
    float m_fRandVal;
    cCom_Animator2D* m_pAnimator;
};

