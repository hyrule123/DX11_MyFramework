#pragma once
#include "CUI_BasicWIndow.h"

class CGameObject;
class CUI_Tree;
class CUI_Inspector;

//현재 레벨에 존재하는 게임오브젝트의 계층구조를 일괄적으로 가져옴.
class CUI_Outliner :
    public CUI_BasicWindow
{
public:
    CUI_Outliner();
    virtual ~CUI_Outliner();

public:
    virtual void init() override;
    virtual void tick() override;

private:
    CUI_Tree* m_arrLayer[MAX_LAYER];

    CUI_Inspector* m_pInspectorUI;

public:

public:
    void UpdateObjectTree();

private:
    void LoadGameObjectHierarchy(CUI_Tree* _pUI, CGameObject* _pGameObject);
    void CallbackLinkInspector(tUIData _UIData);
};

