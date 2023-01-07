#pragma once
#include "CEntity.h"

#include "CGameObject.h"

class CTransform;
class CMeshRender;
class CCamera;

class CComponent :
    public CEntity
{
private:
    CGameObject*         m_pOwner;
    const eCOMPONENT_TYPE m_Type;

public:
    eCOMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_pOwner ; }

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init() {}
    virtual void tick() {}
    virtual void finaltick() = 0;
    virtual CComponent* Clone() = 0;

public:
    CTransform* Transform() { return m_pOwner->Transform(); }
    CMeshRender* MeshRender() { return m_pOwner->MeshRender(); }
    CCamera* Camera() { return m_pOwner->Camera(); }



public:
    CComponent(eCOMPONENT_TYPE _Type);
    virtual ~CComponent();
    friend class CGameObject;
};

