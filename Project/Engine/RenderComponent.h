#pragma once

#include "global.h"
#include "define.h"

#include "Component.h"


#include "Ptr.h"

#include "Mesh.h"
#include "Material.h"



class Camera;
class RenderComponent :
    public Component
{
public:
    RenderComponent(eCOMPONENT_TYPE _type);

    RenderComponent(const RenderComponent& _other);
    CLONE_DISABLE(RenderComponent);

    virtual ~RenderComponent();


public:
    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;



public:
    //이 클래스를 상속받는 하위 컴포넌트들은 인스턴싱을 하는지 아닌지 여부에 따라서 각자 설정해줘야함.
    //인자 : 현재 render를 호출한 카메라의 번호
    //반환값 : 인스턴싱 여부(드로우콜이 일어날 경우 true 반환, 인스턴싱을 위한 데이터 전달만 했을 경우 false 반환.)
    virtual bool render() = 0;

protected:
    //상수버퍼에 재질 데이터를 업로드 및 바인딩
    void BindMtrlScalarDataToCBuffer();

private:
    Ptr<Mesh>              m_pMesh;

    //원본 재질. 특별한 상태를 표현할 필요가 없을 경우 이 재질을 사용
    Ptr<Material>          m_pSharedMtrl;

    //SharedMaterial 복사본(Clone). 고유 상태 표현이 필요할 경우 이 재질을 사용
    Ptr<Material>          m_pDynamicMtrl;

    //현재 사용 중인 재질 주소
    Ptr<Material>          m_pCurrentMtrl;

    bool                    m_bIsDynamicMode;

public:
    void SetMesh(Ptr<Mesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<Material> _Mtrl);

    Ptr<Mesh> GetMesh() { return m_pMesh; }
    Ptr<Material> GetCurMaterial() { return m_pCurrentMtrl; }
    Ptr<Material> GetSharedMaterial();
    Ptr<Material> GetDynamicMaterial();

    const tMtrlScalarData& GetMtrlScalarData() const { return GetOwner()->GetMtrlScalarData(); }
    bool IsRenderReady() { return ((nullptr != m_pMesh) && (nullptr != m_pSharedMtrl)); }

    void SetCamIdx(eCAMERA_INDEX _eCamIdx);
};


inline void RenderComponent::SetMaterial(Ptr<Material> _Mtrl)
{
    m_pSharedMtrl = _Mtrl;

    if (nullptr == _Mtrl)
        return;

    //인스턴싱을 사용하지 않으면 무조건 복사본을 생성
    if (false == m_pSharedMtrl->IsUseInstancing())
    {
        GetDynamicMaterial();
    }
    else
    {
        m_pCurrentMtrl = m_pSharedMtrl;
    }
}

inline Ptr<Material> RenderComponent::GetSharedMaterial()
{ 
    m_pCurrentMtrl = m_pSharedMtrl;
    m_bIsDynamicMode = false;
    return m_pSharedMtrl; 
}
