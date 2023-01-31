#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CCollider2D;
class CMeshRender;
class CCamera;
class CRenderComponent;
class CScriptHolder;
class CScript;
class CLight2D;
class CTilemap;

class CGameObject :
    public CEntity
{
private:

    //Components
    CComponent*             m_arrCom[eCOMPONENT_END];
    CRenderComponent*       m_RenderCom;

    //Hierarchy
    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChild;

    int                     m_iLayerIdx;
    bool                    m_bFixLayer;    //레이어 번호를 고정. 부모 레이어를 옮겨도 자신은 옮겨지지 않음.


    bool                    m_bDestroy;
    float                   m_fLifeSpan;

    //초기화 되어 현재 Level 안에서 작동중인지 여부를 저장.
    //작동 이후 컴포넌트가 추가될 시 바로 init 호출.
    bool                m_bInitialized;

public:
    //Inline Setter
    void                SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }
    void                SetLayerFixed(bool _bFix) { m_bFixLayer = _bFix; }
    void                SetParent(CGameObject* _pObj) { m_Parent = _pObj; }
    void                DestroyForEventMgr();
    void                SetLifeSpan(float _fLifeSpan);
    void                SetChildTransformToUpdate();

    //Inline Getter
    ////Components
    CComponent* GetComponent(eCOMPONENT_TYPE _type) const { return (CComponent*)m_arrCom[_type]; }

    CTransform* Transform() const { return (CTransform*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_TRANSFORM]; }

    CCollider2D* Collider2D() const { return (CCollider2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_COLLIDER2D]; }

    CMeshRender* MeshRender() const { return (CMeshRender*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_MESH_RENDER]; }
    CTilemap* Tilemap() const { return (CTilemap*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_TILEMAP]; }

    CCamera* Camera() const { return (CCamera*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_CAMERA]; }
    CRenderComponent* GetRenderComponent() const { return m_RenderCom; }
    CScriptHolder* ScriptHolder() const { return (CScriptHolder*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_SCRIPT_HOLDER]; }
    CLight2D* Light2D() const { return (CLight2D*)(m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_LIGHT2D]); }

    CGameObject* GetParent() const { return m_Parent; }
    int          GetLayer() const { return m_iLayerIdx; }
    bool         GetDestroyed() const { return m_bDestroy; }
    
    //Master GameObject만 LevelMgr에서 tick()를 호출한다.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    //Add
    void AddComponent(CComponent* _Component);
    void AddScript(CScript* _Script);
    void AddChild(CGameObject* _Object);
    void RemoveChild(CGameObject* _Object);

    //Recursive
    void AddAllHierarchyObjects(__in int _iLayerIdx, __out vector<CGameObject*>& _vecObj);
    void SetParentTransformUpdated();

    //기타
    bool GetParentWorldMatrix(__out Matrix& _mat);

    

public:
    void init();
    void tick();
    virtual void finaltick();
    void render();

    //제거되기 전 기존 오브젝트들과의 관계를 제거
    //bDestroy가 true일 경우 위의 로직은 진행하지 않고 오직 cleanup() 함수만 호출한다.
    void cleanup();


    CLONE(CGameObject)    
public:
    CGameObject();
    CGameObject(const CGameObject& _other);
    ~CGameObject();
};

inline void CGameObject::DestroyForEventMgr()
{
    m_bDestroy = true;

    size_t size = m_vecChild.size();
    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->DestroyForEventMgr();
    }
}

inline void CGameObject::SetLifeSpan(float _fLifeSpan)
{
    //이미 수명이 설정되었을 경우에는 retrurn
    if (m_fLifeSpan != FLT_MAX_NEG)
        return;

    m_fLifeSpan = _fLifeSpan;
}


