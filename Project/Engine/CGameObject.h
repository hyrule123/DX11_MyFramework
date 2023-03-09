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
class CAnimator2D;
class CCollider3D;

class CGameObject :
    public CEntity
{
    CLONE(CGameObject)
public:
    CGameObject();
    CGameObject(const CGameObject& _other);
    ~CGameObject();

public:
    void init();
    void tick();
    virtual void finaltick();
    bool render();

    //���ŵǱ� �� ���� ������Ʈ����� ���踦 ����
    //bDestroy�� true�� ��� ���� ������ �������� �ʰ� ���� cleanup() �Լ��� ȣ���Ѵ�.
    void cleanup();

private:
    //Components
    CComponent*             m_arrCom[(UINT)eCOMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;

    //Hierarchy
    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChild;

    //Own Scalar Data
    tMtrlScalarData          m_MtrlScalarData;

    //Layer Info
    int                     m_iLayerIdx;
    bool                    m_bFixLayer;    //���̾� ��ȣ�� ����. �θ� ���̾ �Űܵ� �ڽ��� �Ű����� ����.

    //Birth, Death
    bool                    m_bDestroy;
    float                   m_fLifeSpan;

    //�ʱ�ȭ �Ǿ� ���� Level �ȿ��� �۵������� ���θ� ����.
    //�۵� ���� ������Ʈ�� �߰��� �� �ٷ� init ȣ��.
    bool                m_bInitialized;

public:
    void                SetLayerIdx(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }
    void                SetLayerFixed(bool _bFix) { m_bFixLayer = _bFix; }
    void                SetParent(CGameObject* _pObj) { m_Parent = _pObj; }
    void                DestroyForEventMgr();
    void                SetLifeSpan(float _fLifeSpan);
    void                SetChildTransformToUpdate();

    bool                IsInitialized() const { return m_bInitialized; }

    ////Components
    CComponent* GetComponent(eCOMPONENT_TYPE _type) const { return (CComponent*)m_arrCom[(UINT)_type]; }

    CTransform* Transform() const { return (CTransform*)m_arrCom[(UINT)eCOMPONENT_TYPE::TRANSFORM]; }

    CCollider2D* Collider2D() const { return (CCollider2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER2D]; }
    CCollider3D* Collider3D() const { return (CCollider3D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER3D]; }

    CAnimator2D* Animator2D() const { return (CAnimator2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]; }

    CMeshRender* MeshRender() const { return (CMeshRender*)m_arrCom[(UINT)eCOMPONENT_TYPE::MESH_RENDER]; }
    CTilemap* Tilemap() const { return (CTilemap*)m_arrCom[(UINT)eCOMPONENT_TYPE::TILEMAP]; }

    CCamera* Camera() const { return (CCamera*)m_arrCom[(UINT)eCOMPONENT_TYPE::CAMERA]; }
    CRenderComponent* GetRenderComponent() const { return m_RenderCom; }
    CScriptHolder* ScriptHolder() const { return (CScriptHolder*)m_arrCom[(UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER]; }
    CLight2D* Light2D() const { return (CLight2D*)(m_arrCom[(UINT)eCOMPONENT_TYPE::LIGHT2D]); }

    CGameObject* GetParent() const { return m_Parent; }
    int          GetLayer() const { return m_iLayerIdx; }
    bool         GetDestroyed() const { return m_bDestroy; }
    
    //Master GameObject�� LevelMgr���� tick()�� ȣ���Ѵ�.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    //Add
    void AddComponent(CComponent* _Component);
    void AddScript(CScript* _Script);
    void AddChildObj(CGameObject* _Object);
    void RemoveChild(CGameObject* _Object);

    //Recursive
    void AddAllHierarchyObjects(__in int _iLayerIdx, __out vector<CGameObject*>& _vecObj);
    void SetParentMatrixUpdated();

    //��Ÿ
    bool GetParentWorldMatrix(__out Matrix& _mat);

    const vector<CGameObject*>& GetvecChilds() const { return m_vecChild; }
    size_t GetNumChilds() const { return m_vecChild.size(); }


    void SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);
    void SetMtrlScalarParam_IntFlag(eMTRLDATA_PARAM_SCALAR _intParam, INT32 _iFlag, bool _bOnOff);

    
    int GetMtrlScalarParam_Int(eMTRLDATA_PARAM_SCALAR _Param) const;
    float GetMtrlScalarParam_Float(eMTRLDATA_PARAM_SCALAR _Param) const;
    Vec2 GetMtrlScalarParam_Vec2(eMTRLDATA_PARAM_SCALAR _Param) const;
    const Vec4& GetMtrlScalarParam_Vec4(eMTRLDATA_PARAM_SCALAR _Param) const;
    const MATRIX& GetMtrlScalarParam_Matrix(eMTRLDATA_PARAM_SCALAR _Param) const;
    const tMtrlScalarData& GetMtrlScalarData() const { return m_MtrlScalarData; }

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


inline void CGameObject::SetMtrlScalarParam_IntFlag(eMTRLDATA_PARAM_SCALAR _intParam, INT32 _iFlag, bool _bOnOff)
{
    INT32* pFlag = nullptr;

    switch (_intParam)
    {
    case eMTRLDATA_PARAM_SCALAR::INT_0: pFlag = &(m_MtrlScalarData.INT_0); break;

    case eMTRLDATA_PARAM_SCALAR::INT_1: pFlag = &(m_MtrlScalarData.INT_1); break;

    case eMTRLDATA_PARAM_SCALAR::INT_2: pFlag = &(m_MtrlScalarData.INT_2); break;

    case eMTRLDATA_PARAM_SCALAR::INT_3: pFlag = &(m_MtrlScalarData.INT_3); break;

    default: return;
    }

    if (true == _bOnOff)
        (*pFlag) |= _iFlag;
    else
        (*pFlag) &= ~_iFlag;
}


inline int CGameObject::GetMtrlScalarParam_Int(eMTRLDATA_PARAM_SCALAR _Param) const
{
    switch (_Param)
    {
    case eMTRLDATA_PARAM_SCALAR::INT_0: return m_MtrlScalarData.INT_0;
        
    case eMTRLDATA_PARAM_SCALAR::INT_1: return m_MtrlScalarData.INT_1;
        
    case eMTRLDATA_PARAM_SCALAR::INT_2: return m_MtrlScalarData.INT_2;
        
    case eMTRLDATA_PARAM_SCALAR::INT_3: return m_MtrlScalarData.INT_3;
        

    default: return INT_MIN;
    }
}

inline float CGameObject::GetMtrlScalarParam_Float(eMTRLDATA_PARAM_SCALAR _Param) const
{
    switch (_Param)
    {
    case eMTRLDATA_PARAM_SCALAR::FLOAT_0: return m_MtrlScalarData.FLOAT_0;

    case eMTRLDATA_PARAM_SCALAR::FLOAT_1: return m_MtrlScalarData.FLOAT_1;

    case eMTRLDATA_PARAM_SCALAR::FLOAT_2: return m_MtrlScalarData.FLOAT_2;

    case eMTRLDATA_PARAM_SCALAR::FLOAT_3: return m_MtrlScalarData.FLOAT_3;


    default: return FLT_MAX_NEG;
    }
}

inline Vec2 CGameObject::GetMtrlScalarParam_Vec2(eMTRLDATA_PARAM_SCALAR _Param) const
{
    switch (_Param)
    {
    case eMTRLDATA_PARAM_SCALAR::VEC2_0: return m_MtrlScalarData.VEC2_0;

    case eMTRLDATA_PARAM_SCALAR::VEC2_1: return m_MtrlScalarData.VEC2_1;

    case eMTRLDATA_PARAM_SCALAR::VEC2_2: return m_MtrlScalarData.VEC2_2;

    case eMTRLDATA_PARAM_SCALAR::VEC2_3: return m_MtrlScalarData.VEC2_3;

    default: return Vec2(FLT_MAX_NEG);
    }
}

inline const Vec4& CGameObject::GetMtrlScalarParam_Vec4(eMTRLDATA_PARAM_SCALAR _Param) const
{
    switch (_Param)
    {
    case eMTRLDATA_PARAM_SCALAR::VEC4_0: return m_MtrlScalarData.VEC4_0;

    case eMTRLDATA_PARAM_SCALAR::VEC4_1: return m_MtrlScalarData.VEC4_1;

    case eMTRLDATA_PARAM_SCALAR::VEC4_2: return m_MtrlScalarData.VEC4_2;

    case eMTRLDATA_PARAM_SCALAR::VEC4_3: return m_MtrlScalarData.VEC4_3;

    default: return Vec4(FLT_MAX_NEG);
    }
}

inline const MATRIX& CGameObject::GetMtrlScalarParam_Matrix(eMTRLDATA_PARAM_SCALAR _Param) const
{
    switch (_Param)
    {
    case eMTRLDATA_PARAM_SCALAR::MAT_0: return m_MtrlScalarData.MAT_0;

    case eMTRLDATA_PARAM_SCALAR::MAT_1: return m_MtrlScalarData.MAT_1;

    case eMTRLDATA_PARAM_SCALAR::MAT_2: return m_MtrlScalarData.MAT_2;

    case eMTRLDATA_PARAM_SCALAR::MAT_3: return m_MtrlScalarData.MAT_3;

    default: return MATRIX::Zero;
    }
}