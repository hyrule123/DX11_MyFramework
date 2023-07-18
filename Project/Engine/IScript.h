#pragma once
#include "IComponent.h"

#include "cScriptHolder.h"

class cGameObject;
class ICollider;

class IScript :
    public IEntity
{
public:
    IScript(const string_view _strKey);

    //복사 생성자는 필요시 재정의할 것
    IScript(const IScript& _other);

    //CLONE(스크립트 이름) 을 사용하면 재정의 가능.
    virtual IScript* Clone() = 0;

    virtual ~IScript();
public:
    //strKey는 생성될 때 정해지므로 여기선 json 저장 불러오기를 해줄것이 없음
    //virtual bool SaveJson(Json::Value* _jVal) override {  return IEntity::SaveJson(_jVal); }
    //virtual bool LoadJson(Json::Value* _jVal) override { return IEntity::SaveJson(_jVal); }

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init()  {}
    virtual void start() {}
    virtual void tick()  {}

    //보낼 GPU 데이터가 있을 경우 데이터를 전송
    virtual void BindData() {}
    virtual void UnBind() {}

    //충돌 발생 시 처리는 이 메소드를 오버라이딩해서 사용해주면 된다.
    virtual void BeginCollision(ICollider* _other, const Vec3& _v3HitPoint) {}
    virtual void OnCollision(ICollider* _other, const Vec3& _v3HitPoint) {}
    virtual void EndCollision(ICollider* _other) {}

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
    IRenderer* Renderer() const { return m_pHolder->Renderer(); }
    cCom_Camera* Camera() const { return m_pHolder->Camera(); }
    cCom_Animator2D* Animator2D() const { return m_pHolder->Animator2D(); }

    cGameObject* GetOwner() const { return m_pHolder->GetOwner(); }
};

