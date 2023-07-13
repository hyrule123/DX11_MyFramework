#pragma once
#include "cComponent.h"

#include "Ptr.h"

class cMesh;
class cMaterial;

class cCamera :
    public cComponent
{
public:
    cCamera();
    cCamera(const cCamera& _other);
    ~cCamera();

    CLONE(cCamera)

private:
    float m_AspectRatio;
    ePROJ_TYPE m_eProjectionType;

    Matrix m_matView;
    Matrix m_matProj;

    float m_fZoomScale;

    int m_iCamIdx;
    
    UINT32 m_LayerFlag;

public:
    void SetProjType(ePROJ_TYPE _Type);
    ePROJ_TYPE GetProjType() const { return m_eProjectionType; }


    void SetCamIndex(eCAMERA_INDEX _Idx);
    eCAMERA_INDEX GetCamIndex() const { return (eCAMERA_INDEX)m_iCamIdx; }

    void SetLayerFlag(UINT32 _iLayerFlag);
    void AddLayerFlag(UINT32 _iLayerNum);
    void OffLayerFlag(UINT32 _iLayerNum);

    const Matrix& GetViewMatrix() const { return m_matView; }
    Matrix GetViewProjMatrix() const { return (m_matView * m_matProj); }

public:
    void Zoom2D(float _fScale);

public:
    virtual void init() override;
    virtual void finaltick() override;
    virtual void cleanup() override;

    //자신 기준으로 찍어야 할 오브젝트들을 분류한다.
    void SortObject();
};

inline void cCamera::SetLayerFlag(UINT32 _iLayerFlag)
{
    m_LayerFlag = _iLayerFlag;
}

inline void cCamera::AddLayerFlag(UINT32 _iLayerNum)
{
    if (_iLayerNum > 32u)
        return;

    m_LayerFlag |= 1 << _iLayerNum;
}

inline void cCamera::OffLayerFlag(UINT32 _iLayerNum)
{
    if (_iLayerNum > 32u)
        return;

    m_LayerFlag &= ~(1 << _iLayerNum);
}
