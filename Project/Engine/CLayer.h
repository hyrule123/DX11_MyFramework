#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    list<CGameObject*>    m_listObject;

public:    
    void tick();
    void finaltick();


public:
    void AddGameObject(CGameObject* _Object) { m_listObject.push_back(_Object); }
    const list<CGameObject*>& GetObjList() const { return m_listObject; }



    CLONE(CLayer)
public:
    CLayer();
    ~CLayer();
};

