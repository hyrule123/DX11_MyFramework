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
    CComponent*         m_arrCom[eCOMPONENT_END];
    CRenderComponent*   m_RenderCom;

    //Hierarchy
    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChild;

    int                     m_iLayerIdx;
    bool                    m_bFixLayer;    //���̾� ��ȣ�� ����. �θ� ���̾ �Űܵ� �ڽ��� �Ű����� ����.

    //�ʱ�ȭ �Ǿ� ���� Level �ȿ��� �۵������� ���θ� ����.
    //�۵� ���� ������Ʈ�� �߰��� �� �ٷ� init ȣ��.
    bool                m_bInitialized;

public:
    //Inline Setter
    void                SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }
    void                SetLayerFixed(bool _bFix) { m_bFixLayer = _bFix; }

    //Inline Getter
    ////Components
    CTransform*         Transform() const { return (CTransform*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_TRANSFORM]; }
    CMeshRender*        MeshRender() const { return (CMeshRender*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_MESH_RENDER]; }
    CCamera*            Camera() const { return (CCamera*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_CAMERA]; }
    CRenderComponent*   GetRenderComponent() const { return m_RenderCom; }
    CScriptHolder*      ScriptHolder() const { return (CScriptHolder*)m_arrCom[(UINT)eCOMPONENT_TYPE::eCOMPONENT_SCRIPT_HOLDER]; }

    CGameObject* GetParent() const { return m_Parent; }
    int          GetLayer() const { return m_iLayerIdx; }
    
    //Master GameObject�� LevelMgr���� tick()�� ȣ���Ѵ�.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    //Add
    void AddComponent(CComponent* _Component);
    void AddScript(CScript* _Script);
    void AddChild(CGameObject* _Object);

    //Recursive
    void AddAllHierarchyObjects(__in int _iLayerIdx, __out list<CGameObject*>& _listObj);

    //��Ÿ
    bool                GetParentWorldMatrix(__out Matrix& _mat);

private:
    void RemoveChild(CGameObject* _Object);

public:
    void init();
    void tick();
    void finaltick();
    void render();


    CLONE(CGameObject)    
public:
    CGameObject();
    CGameObject(const CGameObject& _other);
    ~CGameObject();

    //��� private ���� ȣ���� ���� GameObject������ friend ����
    friend class CGameObject;
};
