#pragma once
#include "IEntity.h"

#include "define.h"

class cGameObject;
class IComponent :
    public IEntity
{
    friend class cGameObject;

public:
    IComponent(eCOMPONENT_TYPE _Type);
    virtual ~IComponent();

    IComponent(const IComponent& _other);
    virtual IComponent* Clone() = 0;

public:
    virtual bool SaveJson(Json::Value* _pJson) override;
    virtual bool LoadJson(Json::Value* _pJson) override;


public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void Init() {}
    virtual void Start() {}
    virtual void Tick() {}
    virtual void FinalTick() = 0;

    //finaltick이 끝난 후에 호출됨. 초기화해줘야 할 더티플래그가 있을 경우 재정의해서 사용할것.
    virtual void ClearDirtyFlags() {};

    virtual void CleanUp() = 0;

    

private:
    cGameObject*            m_pOwner;
    const eCOMPONENT_TYPE   m_eComponentType;
    bool                    m_bIsDisabled;

public:
    //inline Setter
    //소유자 포인터를 하위 클래스에게도 전달해줘야 할 경우 재정의해서 사용해야 함
    void SetOwner(cGameObject* _pOwner) { m_pOwner = _pOwner; }

    //inline Getter
    eCOMPONENT_TYPE GetType() const { return m_eComponentType; }
    cGameObject* GetOwner() const { return m_pOwner ; }

    SETTER(bool, m_bIsDisabled, Disable);
    bool isDisabled() const { return m_bIsDisabled; }
};

