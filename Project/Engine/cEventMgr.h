#pragma once
#include "Singleton.h"

#include "global.h"
#include "struct.h"

class cGameObject;
class cEventMgr
    : public Singleton<cEventMgr>
{
    SINGLETON(cEventMgr);

public:
    void Tick();
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
    //lParam = cGameObject Pointer
    void SpawnNewGameObj(const tGameEvent& _event);

    //lParam = cGameObject Pointer
    //wParam = None
    void DestroyGameObj(const tGameEvent& _event);

    //lParam = Parent cGameObject*
    //wParam = Child cGameObject*
    //SpawnNewGameObj는 따로 해주지 않으므로 주의
    void AddChildGameObj(const tGameEvent& _event);

    //lParam = cGameObject*
    //wParam = eCOMPONENT_TYPE
    void RemoveComponent(const tGameEvent& _event);
    void RemoveComponentLazy(const tGameEvent& _event);


private:
    void ProcessEvent();
    void ProcessLazyEvent();
};

