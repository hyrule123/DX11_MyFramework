#pragma once
#include "CEntity.h"
#include "global.h"

#include "CLayer.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
public:
    CLevel();

    CLevel(const CLevel& _other) = default;
    CLONE_DISABLE(CLevel);

    virtual ~CLevel();

public:
    void tick();
    void finaltick();

private:
    CLayer     m_arrLayer[MAX_LAYER];
    eLEVEL_STATE m_CurState;

public:
    void SetLayerName(int _iLayer, const string& _sLayerName);
    const string& GetLayerName(int _iLayer);

    int GetLayerIdxByName(const string& _sLayerName);
    CLayer& GetLayer(UINT _iLayer);

    CGameObject* FindObjectByName(const string& _Name);
    void FindObjectALLByName(const string& _Name, vector<CGameObject*>& _out);

    void SetState(eLEVEL_STATE _eState);

    //새 게임 오브젝트를 추가.(기존에 게임에 있던 child 오브젝트를 옮길 경우 쓰지 말 것)
    void AddNewGameObj(CGameObject* _Object);

    //Remove
    void RemoveDestroyed();

    void AddNewGameObj_Recursive(CGameObject* _pObj);
};

inline CLayer& CLevel::GetLayer(UINT _iLayer)
{
    assert(0 <= _iLayer && _iLayer < MAX_LAYER);
    return m_arrLayer[_iLayer];
}

