#pragma once
#include "Component.h"

#include "ScriptHolder.h"

class GameObject;
class Collider;
class CScript_TilemapSC;

class Script :
    public Entity
{
public:
    Script(const string_view _strKey);

    //복사 생성자는 필요시 재정의할 것
    Script(const Script& _other);

    //CLONE(스크립트 이름) 을 사용하면 재정의 가능.
    virtual Script* Clone() = 0;

    virtual ~Script();
public:
    //strKey는 생성될 때 정해지므로 여기선 json 저장 불러오기를 해줄것이 없음
    //virtual bool SaveJson(Json::Value* _jVal) override {  return Entity::SaveJson(_jVal); }
    //virtual bool LoadJson(Json::Value* _jVal) override { return Entity::SaveJson(_jVal); }

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init()  {}
    virtual void start() {}
    virtual void tick()  {}

    //보낼 GPU 데이터가 있을 경우 데이터를 전송
    virtual void BindData() {}
    virtual void UnBind() {}

    //충돌 발생 시 처리는 이 메소드를 오버라이딩해서 사용해주면 된다.
    virtual void BeginCollision(Collider* _other, const Vec3& _v3HitPoint) {}
    virtual void OnCollision(Collider* _other, const Vec3& _v3HitPoint) {}
    virtual void EndCollision(Collider* _other) {}

private:
    const std::string m_strKey;
public:
    const string_view GetKey() const { return m_strKey; }

private:    
    ScriptHolder* m_pHolder;
public:
    virtual void SetHolder(ScriptHolder* _pHolder) { m_pHolder = _pHolder; }
    ScriptHolder* ScriptHolder() const { return m_pHolder; }

private:
    bool m_bDisable;
public:
    void SetEnable(bool _bEnable) { m_bDisable = _bEnable; }
    bool GetEnable() const { return m_bDisable; }
    
public:
    Transform& Transform() const { return m_pHolder->Transform(); }
    MeshRenderer* MeshRenderer() const { return m_pHolder->MeshRenderer(); }
    Camera* Camera() const { return m_pHolder->Camera(); }
    RenderComponent* RenderCom() const { return m_pHolder->RenderComponent(); }
    Animator2D* Animator2D() const { return m_pHolder->Animator2D(); }
    Tilemap* Tilemap() const { return m_pHolder->Tilemap(); }

    GameObject* GetOwner() const { return m_pHolder->GetOwner(); }
};

