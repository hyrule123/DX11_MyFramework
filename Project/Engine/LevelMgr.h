#pragma once
#include "Singleton.h"

class Level;
class GameObject;

class LevelMgr
    : public Singleton<LevelMgr>
{
    SINGLETON(LevelMgr);
private:
    Level*     m_pCurLevel;

public:
    //구현 안 되어 있음
    void SetCurLevel(Level* _pLevel);
    Level* GetCurLevel() const { return m_pCurLevel; }


    void AddNewGameObj(GameObject* _pObj);
    GameObject* FindObjectByName(const string_view _Name);
    void FindObjectALLByName(const string_view _Name, vector<GameObject*>& _vecObj);


public:
    void init();
    void tick();

};

