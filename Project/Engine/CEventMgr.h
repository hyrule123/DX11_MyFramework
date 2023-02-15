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

public:
    void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }

private:
    void CreateObject(const tEvent& _event);
    void DestroyObject(const tEvent& _event);
    void AddChildObj(const tEvent& _event);

public:
    void tick();

    SINGLETON(CEventMgr)
};

