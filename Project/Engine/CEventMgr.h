#pragma once
#include "CSingleton.h"

#include "global.h"
#include "struct.h"

class CGameObject;
class CEventMgr
    : public CSingleton<CEventMgr>
{
    SINGLETON(CEventMgr);

public:
    void tick();
    void cleartick() { m_bLevelModified = false; }

private:
    vector<tGameEvent> m_vecEvent;

    //한 프레임 기다렸다 처리해야 하는 이벤트는 여기에 저장
    vector<tGameEvent> m_vecLazyEvent;

    bool           m_bLevelModified;

public:
    void AddEvent(const tGameEvent& _event) { m_vecEvent.push_back(_event); }
    bool IsLevelChanged() const { return m_bLevelModified; }

private:
    //lParam = CGameObject Pointer
    //rParam = Layer Index
    void CreateObject(const tGameEvent& _event);

    //lParam = CGameObject Pointer
    //rParam = None
    void DestroyObject(const tGameEvent& _event);

    //lParam = Parent GameObject*
    //rParam = Child GameObject*
    //CreateObject는 따로 해주지 않으므로 주의
    void AddChildObj(const tGameEvent& _event);

    //lParam = GameObject*
    //rParam = eCOMPONENT_TYPE
    void RemoveComponent(const tGameEvent& _event);
    void RemoveComponentLazy(const tGameEvent& _event);


private:
    void ProcessEvent();
    void ProcessLazyEvent();
    
    void CreateObjRecursive(CGameObject* _pObj, int _iLayer);

    
};

