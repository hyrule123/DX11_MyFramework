#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*     m_arrLayer[MAX_LAYER];

public:
    void tick();
    void finaltick();
    void render();

public:
    void AddGameObject(CGameObject* _Object, int _iLayerIdx);



    CLONE(CLevel);
public:
    CLevel();
    ~CLevel();
};

