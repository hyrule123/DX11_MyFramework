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
    void AddGameObject(CGameObject* _Object, int _iLayerIdx);
    void SetLayerName(int _iLayer, const wstring& _sLayerName);
    const wstring& GetLayerName(int _iLayer);
    int GetLayerIdxByName(const wstring& _sLayerName);
    CLayer* GetLayer(UINT _iLayer);



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
