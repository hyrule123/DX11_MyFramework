#pragma once
#include "Singleton.h"

#include "global.h"
#include "struct.h"

class GameObject;
class EventMgr
    : public Singleton<EventMgr>
{
    SINGLETON(EventMgr);

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
    //lParam = GameObject Pointer
    void SpawnNewGameObj(const tGameEvent& _event);

    //lParam = GameObject Pointer
    //wParam = None
    void DestroyGameObj(const tGameEvent& _event);

    //lParam = Parent GameObject*
    //wParam = Child GameObject*
    //SpawnNewGameObj는 따로 해주지 않으므로 주의
    void AddChildGameObj(const tGameEvent& _event);

    //lParam = GameObject*
    //wParam = eCOMPONENT_TYPE
    void RemoveComponent(const tGameEvent& _event);
    void RemoveComponentLazy(const tGameEvent& _event);


private:
    void ProcessEvent();
    void ProcessLazyEvent();
};

