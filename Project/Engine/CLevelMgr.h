#pragma once
#include "CSingleton.h"

class CLevel;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{   
    SINGLETON(CLevelMgr);
private:
    CLevel*     m_pCurLevel;

public:
    CLevel* GetCurLevel() const { return m_pCurLevel; }


public:
    void init();
    void tick();

};

