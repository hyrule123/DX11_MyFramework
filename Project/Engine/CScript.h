#pragma once
#include "CComponent.h"

#include "CScriptHolder.h"

class CGameObject;
class CCollider;

class CScript :
    public CEntity
{
public:
    CScript() = delete;
    CScript(std::type_index _TypeIndex);

    CScript(const CScript& _other);
    CLONE(CScript);

    virtual ~CScript();
    

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init() {}
    virtual void tick() {}

    //충돌 발생 시 처리는 이 메소드를 오버라이딩해서 사용해주면 된다.
    virtual void BeginCollision(CCollider* _other) {}
    virtual void OnCollision(CCollider* _other) {}
    virtual void EndCollision(CCollider* _other) {}

private:
    CGameObject* m_pOwner;
    CScriptHolder* m_pHolder;
    const std::type_index m_TypeIndex;

public:
    CGameObject* GetOwner() const { return m_pOwner; }
    void SetOwner(CGameObject* _pObj) { m_pOwner = _pObj; }

    CScriptHolder* GetHolder() const { return m_pHolder; }
    void SetHolder(CScriptHolder* _pHolder) { m_pHolder = _pHolder; }

    CTransform* Transform() const { return m_pOwner->Transform(); }
    CMeshRender* MeshRender() const { return m_pOwner->MeshRender(); }
    CCamera* Camera() const { return m_pOwner->Camera(); }
    CRenderComponent* RenderCom() const { return m_pOwner->RenderComponent(); }
    CAnimator2D* Animator2D() const { return m_pOwner->Animator2D(); }
    const std::type_index GetTypeIndex() const { return m_TypeIndex; }
};

