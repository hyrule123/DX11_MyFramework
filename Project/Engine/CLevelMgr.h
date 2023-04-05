#pragma once
#include "CSingleton.h"

class CLevel;
class CGameObject;

class CLevelMgr
    : public CSingleton<CLevelMgr>
{
    SINGLETON(CLevelMgr);
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

