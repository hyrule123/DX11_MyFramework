#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;
class CCamera;
class CRenderComponent;
class CScriptHolder;
class CScript;

class CGameObject :
    public CEntity
{
private:

    //Components
    CComponent* m_arrCom[eCOMPONENT_END];
    CRenderComponent* m_RenderCom;

    //Hierarchy
    CGameObject* m_Parent;
    vector<CGameObject*>    m_vecChild;

    int                     m_iLayerIdx;
    bool                    m_bFixLayer;    //���̾� ��ȣ�� ����. �θ� ���̾ �Űܵ� �ڽ��� �Ű����� ����.


    bool                    m_bDestroy;
    float                   m_fLifeSpan;

    //�ʱ�ȭ �Ǿ� ���� Level �ȿ��� �۵������� ���θ� ����.
    //�۵� ���� ������Ʈ�� �߰��� �� �ٷ� init ȣ��.
    bool                m_bInitialized;

public:
    //Inline Setter
    void                SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }
    void                SetLayerFixed(bool _bFix) { m_bFixLayer = _bFix; }
    void                SetParent(CGameObject* _pObj) { m_Parent = _pObj; }
    void                DestroyForEventMgr();
    void                SetLifeSpan(float _fLifeSpan);

    //Inline Getter
    ////Components
    CTransform* Transform() const { return (CTransform*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_TRANSFORM]; }
    CMeshRender* MeshRender() const { return (CMeshRender*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_MESH_RENDER]; }
    CCamera* Camera() const { return (CCamera*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_CAMERA]; }
    CRenderComponent* GetRenderComponent() const { return m_RenderCom; }
    CScriptHolder* ScriptHolder() const { return (CScriptHolder*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_SCRIPT_HOLDER]; }

    CGameObject* GetParent() const { return m_Parent; }
    int          GetLayer() const { return m_iLayerIdx; }
    bool         GetDestroyed() const { return m_bDestroy; }
    
    //Master GameObject�� LevelMgr���� tick()�� ȣ���Ѵ�.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    //Add
    void AddComponent(CComponent* _Component);
    void AddScript(CScript* _Script);
    void AddChild(CGameObject* _Object);

    //Recursive
    void AddAllHierarchyObjects(__in int _iLayerIdx, __out vector<CGameObject*>& _vecObj);

    //��Ÿ
    bool                GetParentWorldMatrix(__out Matrix& _mat);

private:
    void RemoveChild(CGameObject* _Object);

public:
    void init();
    void tick();
    virtual void finaltick();
    void render();

    //���ŵǱ� �� ���� ������Ʈ����� ���踦 ����
    //bDestroy�� true�� ��� ���� ������ �������� �ʰ� ���� cleanup() �Լ��� ȣ���Ѵ�.
    void cleanup();


    CLONE(CGameObject)    
public:
    CGameObject();
    CGameObject(const CGameObject& _other);
    ~CGameObject();

    //��� private ���� ȣ���� ���� GameObject������ friend ����
    friend class CGameObject;
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
    //�̹� ������ �����Ǿ��� ��쿡�� retrurn
    if (m_fLifeSpan != FLT_MAX_NEG)
        return;

    m_fLifeSpan = _fLifeSpan;
}
