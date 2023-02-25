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

    //원본 재질. 특별한 상태를 표현할 필요가 없을 경우 이 재질을 사용
    Ptr<CMaterial>          m_pSharedMtrl;  

    //SharedMaterial 복사본(Clone). 고유 상태 표현이 필요할 경우 이 재질을 사용
    //DynamicMtrl은 무조건 단일 드로우콜이 호출됨.
    Ptr<CMaterial>          m_pDynamicMtrl; 

    //현재 사용 중인 재질 주소
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

    //공유 재질을 사용 중일 경우
    bool IsUsingInstancing() const { return (m_pSharedMtrl.Get() == m_pCurrentMtrl.Get()); }

    void SetMtrlScalarParam(const tMtrlScalarData& _tMtrlScalarData);

public:
    //이 클래스를 상속받는 하위 컴포넌트들은 인스턴싱을 하는지 아닌지 여부에 따라서 각자 설정해줘야함.
    //인자 : 현재 render를 호출한 카메라의 번호
    //반환값 : drawcall 여부(인스턴싱일 경우 바로 drawcall을 호출하지 않으므로)
    virtual bool render(eCAMERA_INDEX _eCamIdx) = 0;


};


inline void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
    m_pSharedMtrl = _Mtrl;

    //인스턴싱을 사용하지 않으면 무조건 복사본을 생성
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