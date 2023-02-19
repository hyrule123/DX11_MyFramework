#pragma once

#include "global.h"
#include "define.h"

#include "CComponent.h"


#include "ptr.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;

    Ptr<CMaterial>          m_pSharedMtrl;  //원본 재질. 특별한 상태를 표현할 필요가 없을 경우 이 재질을 사용
    Ptr<CMaterial>          m_pDynamicMtrl; //SharedMaterial 복사본. 고유 상태 표현이 필요할 경우 이 재질을 사용
    Ptr<CMaterial>          m_pCurrentMtrl; //현재 사용 중인 재질 주소

    tMtrlScalarData         m_MyMtrlScalarData;//자신의 스칼라 값

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pCurrentMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();
    bool GetRenderReady() { return ((nullptr != m_pMesh) && (nullptr != m_pSharedMtrl)); }

public:
    void Upload

    void render();


    virtual void render_update();


private:


public:
    CRenderComponent(eCOMPONENT_TYPE _type);
    CRenderComponent(const CRenderComponent& _other);
    virtual ~CRenderComponent();

};


inline void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
    m_pSharedMtrl = _Mtrl;
    m_pCurrentMtrl = m_pSharedMtrl;
}

inline Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{ 
    m_pCurrentMtrl = m_pSharedMtrl;
    return m_pSharedMtrl; 
}