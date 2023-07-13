#pragma once
#include "Singleton.h"

class cLevel;
class cGameObject;

class cLevelMgr
    : public Singleton<cLevelMgr>
{
    SINGLETON(cLevelMgr);
private:
    cLevel*     m_pCurLevel;

public:
    //구현 안 되어 있음
    void SetCurLevel(cLevel* _pLevel);
    cLevel* GetCurLevel() const { return m_pCurLevel; }


    void AddNewGameObj(cGameObject* _pObj);
    cGameObject* FindObjectByName(const string_view _Name);
    void FindObjectALLByName(const string_view _Name, vector<cGameObject*>& _vecObj);


public:
    void init();
    void tick();

};

