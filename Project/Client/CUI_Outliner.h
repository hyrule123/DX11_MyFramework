#pragma once
#include "CUI_BasicWIndow.h"

class CGameObject;
class CUI_Tree;
class CUI_Inspector;

//���� ������ �����ϴ� ���ӿ�����Ʈ�� ���������� �ϰ������� ������.
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
    void CallbackLinkInspector(CUI_Tree* _pUI, tDataPtr _pData);
};

