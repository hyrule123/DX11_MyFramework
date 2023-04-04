#pragma once
#include "Singleton.h"

class CLevel;
class CGameObject;

class CLevelMgr
{
    DECLARE_SINGLETON(CLevelMgr);
private:
    CLevel*     m_pCurLevel;

public:
    void SetCurLevel(CLevel* _pLevel);
    CLevel* GetCurLevel() const { return m_pCurLevel; }

    CGameObject* FindObjectByName(const string& _Name);
    void FindObjectALLByName(const string& _Name, vector<CGameObject*>& _vecObj);


public:
    void init();
    void tick();

};

