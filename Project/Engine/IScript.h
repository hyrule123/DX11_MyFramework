#pragma once
#include "IComponent.h"

class cGameObject;
class ICollider;

class IScript :
    public IComponent
{
public:
    IScript();
    virtual ~IScript();

    //복사 생성자는 필요시 재정의할 것
    IScript(const IScript& _other);

    //CLONE(스크립트 이름) 을 사용하면 재정의 가능.
    virtual IScript* Clone() = 0;


public:
    //strKey는 생성될 때 정해지므로 여기선 json 저장 불러오기를 해줄것이 없음
    //virtual bool SaveJson(Json::Value* _jVal) override {  return IEntity::SaveJson(_jVal); }
    //virtual bool LoadJson(Json::Value* _jVal) override { return IEntity::SaveJson(_jVal); }

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void Init()  {}
    virtual void Start() {}
    virtual void Tick()  {}
    virtual void FinalTick() {}
    virtual void CleanUp() {}

    //보낼 GPU 데이터가 있을 경우 데이터를 전송
    virtual void BindData() {}
    virtual void UnBind() {}

    //충돌 발생 시 처리는 이 메소드를 오버라이딩해서 사용해주면 된다.
    virtual void BeginCollision(ICollider* _other, const Vec3& _v3HitPoint) {}
    virtual void OnCollision(ICollider* _other, const Vec3& _v3HitPoint) {}
    virtual void EndCollision(ICollider* _other) {}
};

