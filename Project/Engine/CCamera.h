#pragma once
#include "CComponent.h"

#include "ptr.h"

class CMesh;
class CMaterial;


struct tRenderQueueData
{
    Ptr<CMaterial> pMtrl;
    Ptr<CMesh> pMesh;
};

class CCamera :
    public CComponent
{
private:
    float m_AspectRatio;
    ePROJ_TYPE m_ProjectionType;

    Matrix m_matView;

    Matrix m_matProj;

    float m_ZoomScale;

    int m_CamIndex;

    vector<CGameObject*>    m_arrvecShaderDomain[(UINT)eSHADER_DOMAIN::_END];
    UINT32 m_LayerFlag;

    vector<tRenderQueueData> m_vecInstancedRenderQueue;

public:
    void SetProjType(ePROJ_TYPE _Type);
    ePROJ_TYPE GetProjType() const { return m_ProjectionType; }

    void AddInstancingRenderQueue(Ptr<CMaterial> _pMtrl, Ptr<CMesh> _pMesh);

    void SetCamIndex(eCAMERA_INDEX _Idx);
    void SetLayerFlag(UINT32 _iLayerFlag);
    void AddLayerFlag(UINT32 _iLayerNum);
    void OffLayerFlag(UINT32 _iLayerNum);

    
    Matrix GetViewProjMatrix() const { return (m_matView * m_matProj); }

public:
    void Zoom2D(float _fScale);

public:
    virtual void init() override;
    virtual void finaltick() override;
    virtual void cleanup() override;
    void SortObject();
    void render();

public:
    CCamera(); 
    CCamera(const CCamera& _other);
    ~CCamera();

    CLONE(CCamera)
};

inline void CCamera::SetLayerFlag(UINT32 _iLayerFlag)
{
    m_LayerFlag = _iLayerFlag;
}

inline void CCamera::AddLayerFlag(UINT32 _iLayerNum)
{
    if (_iLayerNum > 32u)
        return;

    m_LayerFlag |= 1 << _iLayerNum;
}

inline void CCamera::OffLayerFlag(UINT32 _iLayerNum)
{
    if (_iLayerNum > 32u)
        return;

    m_LayerFlag &= ~(1 << _iLayerNum);
}
