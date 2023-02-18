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

    Ptr<CMaterial>          m_pSharedMtrl;  //���� ����. Ư���� ���¸� ǥ���� �ʿ䰡 ���� ��� �� ������ ���
    Ptr<CMaterial>          m_pDynamicMtrl; //SharedMaterial ���纻. ���� ���� ǥ���� �ʿ��� ��� �� ������ ���
    Ptr<CMaterial>          m_pCurrentMtrl; //���� ��� ���� ���� �ּ�

        
    tMtrlScalarData         m_MyMtrlScalarData;//�ڽ��� ��Į�� ��

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pCurrentMtrl; }
    Ptr<CMaterial> GetSharedMaterial() { return m_pSharedMtrl; }
    Ptr<CMaterial> GetDynamicMaterial();
    bool GetRenderReady() { return ((nullptr != m_pMesh) && (nullptr != m_pSharedMtrl)); }

public:
    virtual void render() = 0;

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
