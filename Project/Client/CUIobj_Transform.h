#pragma once
#include "CUIobj_Component.h"


class CUIobj_Transform :
    public CUIobj_Component
{
public:
    CUIobj_Transform();
    virtual ~CUIobj_Transform();

public:
    //Transform은 기본적으로 생성되므로 이 기능을 구현할 필요 없음.
    virtual void CreateNewComUI() override {};
    virtual void EditComUI() override;

    //virtual void render_update() override;
};

