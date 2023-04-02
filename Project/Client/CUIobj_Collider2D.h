#pragma once
#include "CUIobj_Component.h"

class CUI_ComboBox;
class CUIobj_Collider2D :
    public CUIobj_Component
{
public:
    CUIobj_Collider2D();
    virtual ~CUIobj_Collider2D();

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void CreateNewComUI() override;
    virtual void EditComUI() override;

    //virtual bool beginUI() override;
    //virtual void render_update() override;

private:
    CUI_ComboBox* m_ComboColTypeSelector;

    void RectEditUI();
    void CircleEditUI();
};

