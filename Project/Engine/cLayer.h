#pragma once
#include "IEntity.h"

#include "define.h"

class cGameObject;
class cLayer :
    public IEntity
{
public:
    cLayer();
    virtual ~cLayer();
    CLONE(cLayer);

public:
    void tick();
    void finaltick();
    
private:
    vector<cGameObject*>    m_vecObject;
public:
    void AddGameObject(cGameObject* _Object);

    //그냥 지워버리면 댕글링 포인터 되므로 사용에 주의할것!!!
    //레이어 이동에 사용됨.
    void RemoveGameObject(cGameObject* _Object);

    const vector<cGameObject*>& GetvecObj() const { return m_vecObject; }

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
