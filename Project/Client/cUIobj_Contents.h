#pragma once

#include "cUI_BasicWindow.h"
#include <Engine/strKey_Default.h>

class cUI_Tree;

class cUIobj_Contents :
    public cUI_BasicWindow
{
public:
    cUIobj_Contents();
    virtual ~cUIobj_Contents();

public:
    virtual void init() override;
    virtual void Tick() override;

private:
    cUI_Tree* m_arrpResTypeRoot[(int)eRES_TYPE::END];
    cUI_Tree* m_arrpResTypeSelected[(int)eRES_TYPE::END];

public:
    //선택된 리소스를 하이라이팅 해주는 콜백함수
    void ChangeSelectedTreeNode(tUIData _tUIData);

private:
    void UpdateResources();
};

