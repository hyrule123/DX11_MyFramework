#pragma once
#include "CUI_BasicWIndow.h"

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

public:

public:
    void UpdateObjectTree();
};

