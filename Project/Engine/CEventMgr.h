#pragma once
#include "CSingleton.h"

#include "global.h"
#include "struct.h"

class CEventMgr :
    public CSingleton<CEventMgr>
{
private:
    queue<tEvent> m_queueEvent;

public:
    void AddEvent(const tEvent& _event) { m_queueEvent.push(_event); }

public:
    void tick();


    SINGLETON(CEventMgr)
};

