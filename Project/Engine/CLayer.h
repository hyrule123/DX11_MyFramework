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
    float   m_fPresetZDepth;
    bool    m_bYSort;
public:
    void    SetPresetZDepth_Ysort(float _fPresetZDepth, bool eYSortType) { m_fPresetZDepth = _fPresetZDepth; m_bYSort = eYSortType; }
    float   GetPresetZDepth() const { return m_fPresetZDepth; }
    bool    GetYSortType() const { return m_bYSort; }
};
