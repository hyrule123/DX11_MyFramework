#pragma once
#include "CComponent.h"


class CGameObject;

class CScript :
    public CEntity
{
private:
    CGameObject* m_pOwner;
    const std::type_index m_TypeIndex;

public:
    CGameObject* GetOwner() { return m_pOwner; }
    void SetOwner(CGameObject* _pObj) { m_pOwner = _pObj; }
    CTransform* Transform() { return m_pOwner->Transform(); }
    CMeshRender* MeshRender() { return m_pOwner->MeshRender(); }
    CCamera* Camera() { return m_pOwner->Camera(); }
    const std::type_index& GetTypeIndex() { return m_TypeIndex; }

public:
    //특정 변수에서 초기 설정을 해야 할 경우 이 메소드를 오버라이딩해서 사용할 것.
    virtual void init() {}
    virtual void tick() {}
    virtual CScript* Clone() = 0;

public:
    CScript() = delete;
    CScript(const std::type_index& _TypeIndex);
    ~CScript();


};

