#pragma once
#include "cComponent.h"

#include "cScriptHolder.h"

class cGameObject;
class cCollider;

class cScript :
    public cEntity
{
public:
    cScript(const string_view _strKey);

    //복사 생성자는 필요시 재정의할 것
    cScript(const cScript& _other);

    //CLONE(스크립트 이름) 을 사용하면 재정의 가능.
    virtual cScript* Clone() = 0;

    virtual ~cScript();
public:
    //strKey는 생성될 때 정해지므로 여기선 json 저장 불러오기를 해줄것이 없음
    //virtual bool SaveJson(Json::Value* _jVal) override {  return cEntity::SaveJson(_jVal); }
    //virtual bool LoadJson(Json::Value* _jVal) override { return cEntity::SaveJson(_jVal); }

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init()  {}
    virtual void start() {}
    virtual void tick()  {}

    //보낼 GPU 데이터가 있을 경우 데이터를 전송
    virtual void BindData() {}
    virtual void UnBind() {}

    //충돌 발생 시 처리는 이 메소드를 오버라이딩해서 사용해주면 된다.
    virtual void BeginCollision(cCollider* _other, const Vec3& _v3HitPoint) {}
    virtual void OnCollision(cCollider* _other, const Vec3& _v3HitPoint) {}
    virtual void EndCollision(cCollider* _other) {}

private:
    const std::string m_strKey;
public:
    const string_view GetKey() const { return m_strKey; }

private:    
    cScriptHolder* m_pHolder;
public:
    virtual void SetHolder(cScriptHolder* _pHolder) { m_pHolder = _pHolder; }
    cScriptHolder* ScriptHolder() const { return m_pHolder; }

private:
    bool m_bDisable;
public:
    void SetEnable(bool _bEnable) { m_bDisable = _bEnable; }
    bool GetEnable() const { return m_bDisable; }
    
public:
    cTransform& Transform() const { return m_pHolder->Transform(); }
    cMeshRenderer* MeshRenderer() const { return m_pHolder->MeshRenderer(); }
    cCamera* Camera() const { return m_pHolder->Camera(); }
    cRenderComponent* RenderCom() const { return m_pHolder->RenderComponent(); }
    cAnimator2D* Animator2D() const { return m_pHolder->Animator2D(); }
    cTilemap* Tilemap() const { return m_pHolder->Tilemap(); }

    cGameObject* GetOwner() const { return m_pHolder->GetOwner(); }
};

