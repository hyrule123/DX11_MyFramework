#pragma once
#include "CUIobj_Component.h"
class CUIobj_Animator2D :
    public CUIobj_Component
{
public:
    CUIobj_Animator2D();
    virtual ~CUIobj_Animator2D();

public:
    //자신이 담당하는 컴포넌트가 없을 경우 호출되는 함수
    virtual void CreateNewComUI() override;

    //자신이 담당하는 컴포넌트가 있을 경우 호출되는 함수
    virtual void EditComUI() override;
};
