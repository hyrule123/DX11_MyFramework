#pragma once

#include "global.h"
#include "define.h"

#include "CComponent.h"


#include "ptr.h"

#include "CMesh.h"
#include "CMaterial.h"

class CCamera;


class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(eCOMPONENT_TYPE _type);
    CRenderComponent(const CRenderComponent& _other);
    virtual ~CRenderComponent();

private:
    Ptr<CMesh>              m_pMesh;

    //���� ����. Ư���� ���¸� ǥ���� �ʿ䰡 ���� ��� �� ������ ���
    Ptr<CMaterial>          m_pSharedMtrl;  

    //SharedMaterial ���纻(Clone). ���� ���� ǥ���� �ʿ��� ��� �� ������ ���
    //DynamicMtrl�� ������ ���� ��ο����� ȣ���.
    Ptr<CMaterial>          m_pDynamicMtrl; 

    //���� ��� ���� ���� �ּ�
    Ptr<CMaterial>          m_pCurrentMtrl; 

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetCurMaterial() { return m_pCurrentMtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();
    void SetUseInstancing() { GetDynamicMaterial(); }
    bool GetRenderReady() { return ((nullptr != m_pMesh) && (nullptr != m_pSharedMtrl)); }

    //���� ������ ��� ���� ���
    bool IsUsingInstancing() const { return (m_pSharedMtrl.Get() == m_pCurrentMtrl.Get()); }

    void SetMtrlScalarParam(const tMtrlScalarData& _tMtrlScalarData);

public:
    //�� Ŭ������ ��ӹ޴� ���� ������Ʈ���� �ν��Ͻ��� �ϴ��� �ƴ��� ���ο� ���� ���� �����������.
    virtual void render(CCamera* _pCam) = 0;


};


inline void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
    m_pSharedMtrl = _Mtrl;

    //�ν��Ͻ��� ������� ������ ������ ���纻�� ����
    if (false == m_pSharedMtrl->GetInstencedRender())
    {
        GetDynamicMaterial();
        return;
    }

    m_pCurrentMtrl = m_pSharedMtrl;
}

inline Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{ 
    m_pCurrentMtrl = m_pSharedMtrl;
    return m_pSharedMtrl; 
}