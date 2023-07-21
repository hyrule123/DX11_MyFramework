#pragma once
#include "cUIobj_Component.h"

class cUI_ComboBox;
class cUIobj_Collider2D :
    public cUIobj_Component
{
public:
    cUIobj_Collider2D();
    virtual ~cUIobj_Collider2D();

public:
    virtual void init() override;
    virtual void Tick() override;
    virtual void CreateNewComUI() override;
    virtual void EditComUI() override;

    //virtual bool beginUI() override;
    //virtual void render_update() override;

private:
    cUI_ComboBox* m_ComboColTypeSelector;

    void RectEditUI();
    void CircleEditUI();
};

