#pragma once

#include "CUI_BasicWIndow.h"

class CUI_Tree;

class CUI_Contents :
    public CUI_BasicWindow
{
public:
    CUI_Contents();
    virtual ~CUI_Contents();

public:
    virtual void init() override;
    virtual void tick() override;

private:
    CUI_Tree* m_arrpResTypeRoot[(int)eRES_TYPE::END];
    CUI_Tree* m_arrpResTypeSelected[(int)eRES_TYPE::END];

public:
    //���õ� ���ҽ��� ���̶����� ���ִ� �ݹ��Լ�
    void ChangeSelectedTreeNode(CUI_Tree* _pTree, DWORD_PTR _pValue);

private:
    void UpdateResources();
};

