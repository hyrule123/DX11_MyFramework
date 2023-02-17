#pragma once
#include "CSingleton.h"

#include "global.h"
#include "struct.h"

class CEventMgr :
    public CSingleton<CEventMgr>
{
private:
    vector<tEvent> m_vecEvent;

    //���� ��⿭
    //�ٸ� ���ӿ�����Ʈ���� ���� ������ ���ؼ� �� �������� ���� �Ⱓ�� ��.
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

