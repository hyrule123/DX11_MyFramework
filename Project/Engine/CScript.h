#pragma once
#include "CComponent.h"

#include "CScriptHolder.h"

class CGameObject;
class CCollider;
class CTilemap_SC;

class CScript :
    public CEntity
{
public:
    CScript(const string& _strKey);

    CScript(const CScript& _other);
    virtual CScript* Clone() = 0;

    virtual ~CScript();
public:
    //strKey는 생성될 때 정해지므로 여기선 json 저장 불러오기를 해줄것이 없음
    //virtual bool SaveJson(Json::Value* _jVal) override {  return CEntity::SaveJson(_jVal); }
    //virtual bool LoadJson(Json::Value* _jVal) override { return CEntity::SaveJson(_jVal); }

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
    const string m_strKey;
public:
    const string& GetKey() const { return m_strKey; }

private:
    CScriptHolder* m_pHolder;
public:
    virtual void SetHolder(CScriptHolder* _pHolder) { m_pHolder = _pHolder; }
    CScriptHolder* ScriptHolder() const { return m_pHolder; }
    
public:
    CTransform* Transform() const { return m_pHolder->Transform(); }
    CMeshRender* MeshRender() const { return m_pHolder->MeshRender(); }
    CCamera* Camera() const { return m_pHolder->Camera(); }
    CRenderComponent* RenderCom() const { return m_pHolder->RenderComponent(); }
    CAnimator2D* Animator2D() const { return m_pHolder->Animator2D(); }
    CTilemap* Tilemap() const { return m_pHolder->Tilemap(); }

    CGameObject* GetOwner() const { return m_pHolder->GetOwner(); }
};

