#pragma once
#include "cUIobj_Component.h"
class cUIobj_Animator2D :
    public cUIobj_Component
{
public:
    cUIobj_Animator2D();
    virtual ~cUIobj_Animator2D();

public:
    //자신이 담당하는 컴포넌트가 없을 경우 호출되는 함수
    virtual void CreateNewComUI() override;

    //자신이 담당하는 컴포넌트가 있을 경우 호출되는 함수
    virtual void EditComUI() override;
};
