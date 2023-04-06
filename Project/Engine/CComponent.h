#pragma once
#include "CEntity.h"

#include "CGameObject.h"

class CTransform;
class CMeshRender;
class CCamera;
class CCollider3D;

class CComponent :
    public CEntity
{
    friend class CGameObject;

public:
    CComponent(eCOMPONENT_TYPE _Type);
    CComponent(const CComponent& _other);
    virtual ~CComponent();


public:
    virtual bool SaveJson(Json::Value* _pJson) override;
    virtual bool LoadJson(Json::Value* _pJson) override;


public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init() {}
    virtual void tick() {}
    virtual void finaltick() = 0;

    //finaltick이 끝난 후에 호출됨. 초기화해줘야 할 더티플래그가 있을 경우 재정의해서 사용할것.
    virtual void clearDirtyFlags() {};

    virtual void cleanup() = 0;

    virtual CComponent* Clone() = 0;

private:
    CGameObject*         m_pOwner;
    const eCOMPONENT_TYPE m_eComponentType;
    bool                 m_bIsDisabled;

public:
    //inline Setter
    //소유자 포인터를 하위 클래스에게도 전달해줘야 할 경우 재정의해서 사용해야 함
    void SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }

    //inline Getter
    eCOMPONENT_TYPE GetType() const { return m_eComponentType; }
    CGameObject* GetOwner() const { return m_pOwner ; }

    SETTER(bool, m_bIsDisabled, Disable);
    bool isDisabled() const { return m_bIsDisabled; }

public:
    CTransform*         Transform()         { return m_pOwner->Transform(); }
    CMeshRender*        MeshRender()        { return m_pOwner->MeshRender(); }
    CCamera*            Camera()            { return m_pOwner->Camera(); }
    CAnimator2D*        Animator2D()        { return m_pOwner->Animator2D(); }
    CCollider2D*        Collider2D()        { return m_pOwner->Collider2D(); }
    CCollider3D*        Collider3D()        { return m_pOwner->Collider3D(); }
    CRenderComponent*   RenderComponent()   { return m_pOwner->RenderComponent(); }
};

