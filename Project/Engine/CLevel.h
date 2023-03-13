#pragma once
#include "CEntity.h"

#include "global.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
    CLONE(CLevel);
public:
    CLevel();
    ~CLevel();


private:
    CLayer*     m_arrLayer[MAX_LAYER];
    eLEVEL_STATE m_CurState;

public:
    void tick();
    void finaltick();

public:
    void SetLayerName(int _iLayer, const string& _sLayerName);
    const string& GetLayerName(int _iLayer);

    int GetLayerIdxByName(const string& _sLayerName);
    CLayer* GetLayer(UINT _iLayer);

    CGameObject* FindObjectByName(const string& _Name);
    void FindObjectALLByName(const string& _Name, __out vector<CGameObject*>& _out);

    void SetState(eLEVEL_STATE _eState);

    //Add
    void AddGameObject(CGameObject* _Object, int _iLayerIdx);

    //Remove
    void RemoveDestroyed();


};

inline CLayer* CLevel::GetLayer(UINT _iLayer)
{
    if (_iLayer >= MAX_LAYER)
        return nullptr;

    return m_arrLayer[_iLayer];
}

