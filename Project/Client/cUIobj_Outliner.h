#pragma once
#include "cUI_BasicWindow.h"

class cGameObject;
class cUI_Tree;
class cUIobj_Inspector;

//현재 레벨에 존재하는 게임오브젝트의 계층구조를 일괄적으로 가져옴.
class cUIobj_Outliner :
    public cUI_BasicWindow
{
public:
    cUIobj_Outliner();
    virtual ~cUIobj_Outliner();

public:
    virtual void init() override;
    virtual void Tick() override;

private:
    cUI_Tree* m_arrLayer[MAX_LAYER];
    cUIobj_Inspector* m_pInspectorUI;

public:

public:
    void UpdateObjectTree();

private:
    void LoadGameObjectHierarchy(cUI_Tree* _pUI, cGameObject* _pGameObject);
    void CallbackLinkInspector(tUIData _UIData);
};

