#pragma once
#include "CSingleton.h"

class CLevel;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{   
    SINGLE(CLevelMgr);
private:
    CLevel*     m_pCurLevel;

public:
    void init();
    void tick();
    void render();

};

