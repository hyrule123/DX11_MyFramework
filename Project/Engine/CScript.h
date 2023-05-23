#pragma once
#include "CComponent.h"

#include "CScriptHolder.h"

class CGameObject;
class CCollider;

class CScript :
    public CEntity
{
public:
    CScript();

    CScript(const CScript& _other);
    virtual CScript* Clone() = 0;

    virtual ~CScript();
public:
    virtual bool SaveJson(Json::Value* _jVal) override { return true; }
    virtual bool LoadJson(Json::Value* _jVal) override { return true; }

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init()  {}
    virtual void start() {}
    virtual void tick()  {}

    //충돌 발생 시 처리는 이 메소드를 오버라이딩해서 사용해주면 된다.
    virtual void BeginCollision(CCollider* _other, const Vec3& _v3HitPoint) {}
    virtual void OnCollision(CCollider* _other, const Vec3& _v3HitPoint) {}
    virtual void EndCollision(CCollider* _other) {}

private:
    CScriptHolder* m_pHolder;

public:
    CScriptHolder* ScriptHolder() const { return m_pHolder; }
    virtual void SetHolder(CScriptHolder* _pHolder) { m_pHolder = _pHolder; }

    CTransform* Transform() const { return m_pHolder->Transform(); }
    CMeshRender* MeshRender() const { return m_pHolder->MeshRender(); }
    CCamera* Camera() const { return m_pHolder->Camera(); }
    CRenderComponent* RenderCom() const { return m_pHolder->RenderComponent(); }
    CAnimator2D* Animator2D() const { return m_pHolder->Animator2D(); }

    CGameObject* GetOwner() const { return m_pHolder->GetOwner(); }
};

