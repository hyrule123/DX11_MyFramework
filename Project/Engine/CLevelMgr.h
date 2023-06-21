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
    //구현 안 되어 있음
    void SetCurLevel(CLevel* _pLevel);
    CLevel* GetCurLevel() const { return m_pCurLevel; }


    void AddNewGameObj(CGameObject* _pObj);
    CGameObject* FindObjectByName(const string_view _Name);
    void FindObjectALLByName(const string_view _Name, vector<CGameObject*>& _vecObj);


public:
    void init();
    void tick();

};

