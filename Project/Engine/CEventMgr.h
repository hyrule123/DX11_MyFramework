#pragma once
#include "CSingleton.h"

#include "global.h"
#include "struct.h"

class CGameObject;
class CEventMgr :
    public CSingleton<CEventMgr>
{
    SINGLETON(CEventMgr);

public:
    void tick();
    void cleartick() { m_bLevelModified = false; }

private:
    vector<tEvent> m_vecEvent;

    //한 프레임 기다렸다 처리해야 하는 이벤트는 여기에 저장
    vector<tEvent> m_vecLazyEvent;

    bool           m_bLevelModified;

public:
    void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }
    bool IsLevelChanged() const { return m_bLevelModified; }

private:
    //lParam = CGameObject Pointer
    //rParam = Layer Index
    void CreateObject(const tEvent& _event);

    //lParam = CGameObject Pointer
    //rParam = None
    void DestroyObject(const tEvent& _event);

    //lParam = Parent GameObject*
    //rParam = Child GameObject*
    void AddChildObj(const tEvent& _event);

    //lParam = GameObject*
    //rParam = eCOMPONENT_TYPE
    void RemoveComponent(const tEvent& _event);
    void RemoveComponentLazy(const tEvent& _event);


private:
    void ProcessLazyEvent();
    void ProcessEvent();


    
};

