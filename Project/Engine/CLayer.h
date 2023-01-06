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
    void render();


public:
    void AddGameObject(CGameObject* _Object) { m_listObject.push_back(_Object); }


    CLONE(CLayer)
public:
    CLayer();
    ~CLayer();
};

