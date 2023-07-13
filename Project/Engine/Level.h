#pragma once
#include "Entity.h"
#include "global.h"

#include "Layer.h"

class Layer;
class GameObject;

class Level :
    public Entity
{
public:
    Level();

    Level(const Level& _other) = default;
    CLONE_DISABLE(Level);

    virtual ~Level();

public:
    void tick();
    void finaltick();

private:
    Layer     m_arrLayer[MAX_LAYER];
    eLEVEL_STATE m_CurState;

public:
    void SetLayerName(int _iLayer, const string_view _sLayerName);
    const string_view GetLayerName(int _iLayer);

    int GetLayerIdxByName(const string_view _sLayerName);
    Layer& GetLayer(UINT _iLayer);

    GameObject* FindObjectByName(const string_view _Name);
    void FindObjectALLByName(const string_view _Name, vector<GameObject*>& _out);

    void SetState(eLEVEL_STATE _eState);

    //새 게임 오브젝트를 추가.(기존에 게임에 있던 child 오브젝트를 옮길 경우 쓰지 말 것)
    void AddNewGameObj(GameObject* _Object);

    //Remove
    void RemoveDestroyed();

    void AddNewGameObj_Recursive(GameObject* _pObj);
};

inline Layer& Level::GetLayer(UINT _iLayer)
{
    assert(0 <= _iLayer && _iLayer < MAX_LAYER);
    return m_arrLayer[_iLayer];
}

