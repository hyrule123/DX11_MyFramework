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
private:
    CGameObject*         m_pOwner;
    const eCOMPONENT_TYPE m_Type;

public:
    //inline Setter
    //������ �����͸� ���� Ŭ�������Ե� ��������� �� ��� �������ؼ� ����ؾ� ��
    virtual void SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }

    //inline Getter
    eCOMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_pOwner ; }

public:
    //Ư�� �������� �ʱ� ������ �ؾ� �� ��� �� �޼ҵ带 �������̵��ؼ� ����� ��.
    virtual void init() {}
    virtual void tick() {}
    virtual void finaltick() = 0;

    virtual void cleanup() = 0;

    virtual CComponent* Clone() = 0;

public:
    CTransform* Transform() { return m_pOwner->Transform(); }
    CMeshRender* MeshRender() { return m_pOwner->MeshRender(); }
    CCamera* Camera() { return m_pOwner->Camera(); }
    CAnimator2D* Animator2D() { return m_pOwner->Animator2D(); }
    CCollider2D* Collider2D() { return m_pOwner->Collider2D(); }
    CCollider3D* Collider3D() { return m_pOwner->Collider3D(); }
    CRenderComponent* RenderComponent() { return m_pOwner->GetRenderComponent(); }



public:
    CComponent(eCOMPONENT_TYPE _Type);
    CComponent(const CComponent& _other);
    virtual ~CComponent();
    friend class CGameObject;
};

