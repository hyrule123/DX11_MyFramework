#pragma once
#include "CComponent.h"

//#include "CLevelMgr.h"
//#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"





class CScript :
    public CComponent
{
private:


public:
    virtual void finaltick() final {};

public:
    CScript();
    ~CScript();
};

