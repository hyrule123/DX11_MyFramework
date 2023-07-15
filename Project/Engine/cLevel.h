#pragma once
#include "IEntity.h"
#include "global.h"

#include "cLayer.h"

class cLayer;
class cGameObject;

class cLevel :
    public IEntity
{
public:
    cLevel();

    cLevel(const cLevel& _other) = default;
    CLONE_DISABLE(cLevel);

    virtual ~cLevel();

public:
    void tick();
    void finaltick();

private:
    cLayer     m_arrLayer[MAX_LAYER];
    eLEVEL_STATE m_CurState;

public:
    void SetLayerName(int _iLayer, const string_view _sLayerName);
    const string_view GetLayerName(int _iLayer);

    int GetLayerIdxByName(const string_view _sLayerName);
    cLayer& GetLayer(UINT _iLayer);

    cGameObject* FindObjectByName(const string_view _Name);
    void FindObjectALLByName(const string_view _Name, vector<cGameObject*>& _out);

    void SetState(eLEVEL_STATE _eState);

    //새 게임 오브젝트를 추가.(기존에 게임에 있던 child 오브젝트를 옮길 경우 쓰지 말 것)
    void AddNewGameObj(cGameObject* _Object);

    //Remove
    void RemoveDestroyed();

    void AddNewGameObj_Recursive(cGameObject* _pObj);
};

inline cLayer& cLevel::GetLayer(UINT _iLayer)
{
    assert(0 <= _iLayer && _iLayer < MAX_LAYER);
    return m_arrLayer[_iLayer];
}

