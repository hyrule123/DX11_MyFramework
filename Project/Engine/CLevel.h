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

public:
    //Setter
    void SetLayerName(int _iLayer, const string& _sLayerName);

    //Getter
    const string& GetLayerName(int _iLayer);
    int GetLayerIdxByName(const string& _sLayerName);
    CLayer* GetLayer(UINT _iLayer);
    CGameObject* FindObjectByName(const string& _Name);
    void FindObjectALLByName(const string& _Name, __out vector<CGameObject*>& _out);

    //Add
    void AddGameObject(CGameObject* _Object, int _iLayerIdx);

    //Remove
    void RemoveDestroyed();

    CLONE(CLevel);
public:
    CLevel();
    ~CLevel();
};

inline CLayer* CLevel::GetLayer(UINT _iLayer)
{
    if (_iLayer >= MAX_LAYER)
        return nullptr;

    return m_arrLayer[_iLayer];
}

