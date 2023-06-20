#pragma once
#include "CEntity.h"

#include "global.h"

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
    
public:
    CGameObject();

    CGameObject(const CGameObject& _other);
    CLONE(CGameObject);

    virtual ~CGameObject();

public:
    //게임오브젝트 최초 생성 시 호출
    void init();

    //첫 tick 직전에 한 번 호출
    void start();

    //disable 되었다가 enable되었을 떄 호출
    void OnEnable();


    void tick();
    virtual void finaltick();
    bool render();

    //제거되기 전 기존 오브젝트들과의 관계를 제거
    //bDestroy가 true일 경우 위의 로직은 진행하지 않고 오직 cleanup() 함수만 호출한다.
    void cleanup();

public:
    virtual bool SaveJson(Json::Value* _pJson) override;
    virtual bool LoadJson(Json::Value* _pJson) override;

public:

private:
    //Components
    CComponent*             m_arrCom[(UINT)eCOMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;
public:
    //Add
    void AddComponent(CComponent* _Component);

    //CEventMgr 전용 함수. 직접 호출 시 에러 발생할 수 있음.
    void RemoveComponent(eCOMPONENT_TYPE _eComType);

    void AddScript(CScript* _Script);

private:
    //Hierarchy
    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChild;
public:
    void                SetParent(CGameObject* _pObj) { m_Parent = _pObj; }
    CGameObject*        GetParent() const { return m_Parent; }

    //Master GameObject만 LevelMgr에서 tick()를 호출한다.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    void                AddChildGameObj(CGameObject* _Object);

    void                RemoveChild(CGameObject* _Object);


    //기타
    bool                GetParentWorldMatrix(Matrix& _mat);

    const vector<CGameObject*>& GetvecChilds() const { return m_vecChild; }
    size_t              GetNumChilds() const { return m_vecChild.size(); }

    //Recursive
    void                SetParentMatrixUpdated();
    void                SetChildTransformToUpdate();


    //GPU에 보낼 Material Scalar Data
private:
    tMtrlScalarData          m_MtrlScalarData;
public:
    void SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);
    void SetMtrlScalarParam_IntFlag(eMTRLDATA_PARAM_SCALAR _intParam, INT32 _iFlag, bool _bOnOff);


    int GetMtrlScalarParam_Int(eMTRLDATA_PARAM_SCALAR _Param) const;
    float GetMtrlScalarParam_Float(eMTRLDATA_PARAM_SCALAR _Param) const;
    Vec2 GetMtrlScalarParam_Vec2(eMTRLDATA_PARAM_SCALAR _Param) const;
    const Vec4& GetMtrlScalarParam_Vec4(eMTRLDATA_PARAM_SCALAR _Param) const;
    const MATRIX& GetMtrlScalarParam_Matrix(eMTRLDATA_PARAM_SCALAR _Param) const;
    const tMtrlScalarData& GetMtrlScalarData() const { return m_MtrlScalarData; }

    //CCamera에서 호출.
    //Transform에서 등록한 World Matrix의 위치 부분 Z값에 _MinZ(Z 기준값 또는 최솟값) + (Y값 / 현재 해상도)로 덮어씌움
    void YSort(float _MinZ);


private:
    //Layer Info
    int                     m_iLayerIdx;
public:
    void                    SetLayer(int _iLayerIdx) { m_iLayerIdx = _iLayerIdx; }
    int                     GetLayer() const { return m_iLayerIdx; }

private:
    //Birth, Death
    float                   m_fLifeSpan;
public:
    void                    SetLifeSpan(float _fLifeSpan);

private:
    bool                    m_bDestroy;
public:
    //CEventMgr에서 사용. 자식 오브젝트까지 재귀적으로 삭제
    void                    DestroyRecursive();
    bool                    IsDestroyed() const { return m_bDestroy; }

private:
    bool                    m_bStart;

    bool                    m_bEnable;
    bool                    m_bPrevEnable;
    


public:
    ////Components
    CComponent*         GetComponent(eCOMPONENT_TYPE _type) const { return (CComponent*)m_arrCom[(UINT)_type]; }

    CTransform*         Transform() const { return (CTransform*)m_arrCom[(UINT)eCOMPONENT_TYPE::TRANSFORM]; }

    CCollider2D*        Collider2D() const { return (CCollider2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER2D]; }
    CCollider3D*        Collider3D() const { return (CCollider3D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER3D]; }

    CAnimator2D*        Animator2D() const { return (CAnimator2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]; }

    CMeshRender*        MeshRender() const { return (CMeshRender*)m_arrCom[(UINT)eCOMPONENT_TYPE::MESH_RENDER]; }
    CTilemap*           Tilemap() const { return (CTilemap*)m_arrCom[(UINT)eCOMPONENT_TYPE::TILEMAP]; }

    CCamera*            Camera() const { return (CCamera*)m_arrCom[(UINT)eCOMPONENT_TYPE::CAMERA]; }

    CRenderComponent*   RenderComponent() const { return m_RenderCom; }

    CScriptHolder*      ScriptHolder() const { return (CScriptHolder*)m_arrCom[(UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER]; }
    CLight2D* Light2D() const { return (CLight2D*)(m_arrCom[(UINT)eCOMPONENT_TYPE::LIGHT2D]); }
};

inline void CGameObject::DestroyRecursive()
{
    m_bDestroy = true;

    size_t size = m_vecChild.size();
    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->DestroyRecursive();
    }
}

inline void CGameObject::SetLifeSpan(float _fLifeSpan)
{
    //이미 수명이 설정되었을 경우에는 retrurn
    if (m_fLifeSpan != FLT_MAX_NEGATIVE)
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

    case eMTRLDATA_PARAM_SCALAR::INT_4: pFlag = &(m_MtrlScalarData.INT_4); break;

    case eMTRLDATA_PARAM_SCALAR::INT_5: pFlag = &(m_MtrlScalarData.INT_5); break;

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


    default: return FLT_MAX_NEGATIVE;
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

    case eMTRLDATA_PARAM_SCALAR::VEC2_4: return m_MtrlScalarData.VEC2_4;

    case eMTRLDATA_PARAM_SCALAR::VEC2_5: return m_MtrlScalarData.VEC2_5;

    default: return Vec2(FLT_MAX_NEGATIVE);
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

    default: return Vec4(FLT_MAX_NEGATIVE);
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


inline void CGameObject::YSort(float _MaxZ)
{
    enum xyz { x, y, z };
    m_MtrlScalarData.MAT_1.m[3][z] = _MaxZ - (m_MtrlScalarData.MAT_1.m[3][y] / g_GlobalVal.v2Res.y);
}


