#pragma once
#include "CUI_BasicWIndow.h"

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

public:

public:
    void UpdateObjectTree();
};

