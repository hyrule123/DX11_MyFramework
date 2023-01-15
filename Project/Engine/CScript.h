#pragma once
#include "CComponent.h"


class CGameObject;
class CCollider;

class CScript :
    public CEntity
{
private:
    CGameObject* m_pOwner;
    const std::type_index m_TypeIndex;

public:
    CGameObject* GetOwner() const { return m_pOwner; }
    void SetOwner(CGameObject* _pObj) { m_pOwner = _pObj; }
    CTransform* Transform() const { return m_pOwner->Transform(); }
    CMeshRender* MeshRender() const { return m_pOwner->MeshRender(); }
    CCamera* Camera() const { return m_pOwner->Camera(); }
    const std::type_index GetTypeIndex() const { return m_TypeIndex; }

public:
    //Ư�� �������� �ʱ� ������ �ؾ� �� ��� �� �޼ҵ带 �������̵��ؼ� ����� ��.
    virtual void init() {}
    virtual void tick() {}

    virtual void BeginCollision(CCollider* _other) {}
    virtual void OnCollision(CCollider* _other) {}
    virtual void EndCollision(CCollider* _other) {}

public:
    CScript() = delete;
    CScript(std::type_index _TypeIndex);
    CScript(const CScript& _other);
    ~CScript();

    CLONE(CScript)
};

