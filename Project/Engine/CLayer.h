#pragma once
#include "CEntity.h"

#include "define.h"



class CGameObject;
class CLayer :
    public CEntity
{
public:
    CLayer();
    virtual ~CLayer();
    CLONE(CLayer);

public:
    void tick();
    void finaltick();
    
private:
    vector<CGameObject*>    m_vecObject;
public:
    void AddGameObject(CGameObject* _Object);

    //그냥 지워버리면 댕글링 포인터 되므로 사용에 주의할것!!!
    //레이어 이동에 사용됨.
    void RemoveGameObject(CGameObject* _Object);

    const vector<CGameObject*>& GetvecObj() const { return m_vecObject; }

    void RemoveDestroyed();


private:
    int             m_iLayerIdx;
public:
    void SetLayerIdx(int _idx) { m_iLayerIdx = _idx; }

    
private:
    bool    m_bEnableYsorting;
public:
    //이 값의 간격을 너무 좁게하면 YSorting과 동시에 사용 시 깊이 판정에 에러가 발생할 수 있음.
    void SetDepthPreset(float _fDepthPreset, bool _bEnableYsorting) { m_bEnableYsorting = _bEnableYsorting; }

    bool IsYsorting() const { return m_bEnableYsorting; }

private:
    bool Y_Sort(CGameObject* _pObj_L, CGameObject* _pObj_R);
};

