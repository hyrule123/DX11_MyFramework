#pragma once
#include "CComponent.h"

#include "ptr.h"

class CMesh;
class CMaterial;




class CCamera :
    public CComponent
{
public:
    CCamera();
    CCamera(const CCamera& _other);
    ~CCamera();

    CLONE(CCamera)

private:
    float m_AspectRatio;
    ePROJ_TYPE m_ProjectionType;

    Matrix m_matView;

    Matrix m_matProj;

    float m_ZoomScale;

    int m_CamIndex;

    
    UINT32 m_LayerFlag;

    

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

    void UploadData();
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
