#pragma once
#include "CSingleton.h"

#include "global.h"
#include "struct.h"

class CEventMgr :
    public CSingleton<CEventMgr>
{
private:
    vector<tEvent> m_vecEvent;

    //삭제 대기열
    //다른 게임오브젝트와의 관계 정리를 위해서 한 프레임의 유예 기간을 둠.
    vector<CGameObject*> m_vecReserveDestroy;

    bool                 m_bLevelModified;

public:
    void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }
    bool IsLevelChanged() const { return m_bLevelModified; }

private:
    void CreateObject(const tEvent& _event);
    void DestroyObject(const tEvent& _event);
    void AddChildObj(const tEvent& _event);

public:
    void tick();
    void cleartick() { m_bLevelModified = false; }

    SINGLETON(CEventMgr)
};

