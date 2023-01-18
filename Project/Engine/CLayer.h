#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecObject;
    const int             m_iLayerIdx;

public:
    void tick();
    void finaltick();


public:
    void AddGameObject(CGameObject* _Object);

    //�׳� ���������� ��۸� ������ �ǹǷ� ��뿡 �����Ұ�!!!
    void RemoveGameObject(CGameObject* _Object);
    const vector<CGameObject*>& GetObjList() const { return m_vecObject; }



    CLONE(CLayer)
public:
    CLayer(int _iLayerIdx);
    ~CLayer();

private:
    CLayer() = delete;
};

