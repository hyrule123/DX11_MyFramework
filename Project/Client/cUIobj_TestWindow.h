#pragma once
#include "cUI_BasicWindow.h"

class cGameObject;
class cCom_Renderer_TilemapSC;
class cUIobj_TestWindow :
    public cUI_BasicWindow
{
public:
    cUIobj_TestWindow();
    virtual ~cUIobj_TestWindow();

public:
    virtual void init() override;
    virtual void render_update() override;

private:
    cGameObject* m_pMapObj;
    cCom_Renderer_TilemapSC* m_TilemapRenderer;

private:
    void LoadMapData(const struct tComboItem& _tCombo);
    void FindTestObj();
    void ChangeDebugMode(int _iMode);
};

