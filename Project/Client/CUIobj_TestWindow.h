#pragma once
#include "CUI_BasicWindow.h"


class CUIobj_TestWindow :
    public CUI_BasicWindow
{
public:
    CUIobj_TestWindow();
    virtual ~CUIobj_TestWindow();

public:
    virtual void init() override;
    virtual void render_update() override;

private:
    class CGameObject* m_pMapObj;

private:
    void LoadMapData(const struct tComboItem& _tCombo);
    void FindTestObj();
    void ChangeDebugMode(int _iMode);
};

