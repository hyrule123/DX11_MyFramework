#pragma once
#include "IEntity.h"

#include "global.h"

#include "cCom_Transform.h"

class IComponent;
class cCom_Transform;
class ICollider2D;
class cCom_Renderer_Basic;
class cCom_Camera;
class IRenderer;
class cScriptHolder;
class IScript;
class cCom_Light2D;
class ITilemapBase;
class cCom_Animator2D;
class ICollider3D;

class cGameObject :
    public IEntity
{
    
public:
    cGameObject();

    cGameObject(const cGameObject& _other);
    CLONE(cGameObject);

    virtual ~cGameObject();

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
    IComponent*             m_arrCom[(UINT)eCOMPONENT_TYPE::END];

public:
    //Add
    void AddComponent(IComponent* _Component);

    //cEventMgr 전용 함수. 직접 호출 시 에러 발생할 수 있음.
    void RemoveComponent(eCOMPONENT_TYPE _eComType);

    void AddScript(IScript* _Script);

private:
    //Hierarchy
    cGameObject*            m_Parent;
    vector<cGameObject*>    m_vecChild;
public:
    void                SetParent(cGameObject* _pObj) { m_Parent = _pObj; }
    cGameObject*        GetParent() const { return m_Parent; }

    //Master cGameObject만 cLevelMgr에서 tick()를 호출한다.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    void                AddChildGameObj(cGameObject* _Object);

    void                RemoveChild(cGameObject* _Object);


    //기타
    bool                GetParentWorldMatrix(Matrix& _mat);

    const vector<cGameObject*>& GetvecChilds() const { return m_vecChild; }
    size_t              GetNumChilds() const { return m_vecChild.size(); }

    //Recursive
    void                SetParentMatrixUpdated();
    void                SetChildTransformToUpdate();


    //GPU에 보낼 cMaterial Scalar Data
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

    //cCom_Camera에서 호출.
    //cCom_Transform에서 등록한 World Matrix의 위치 부분 Z값에 _MinZ(Z 기준값 또는 최솟값) + (Y값 / 현재 해상도)로 덮어씌움
    void YSort(float _MinZ);


private:
    //cLayer Info
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
    //cEventMgr에서 사용. 자식 오브젝트까지 재귀적으로 삭제
    void                    DestroyRecursive();
    bool                    IsDestroyed() const { return m_bDestroy; }

private:
    bool                    m_bStart;

    bool                    m_bDisable;
    bool                    m_bPrevEnable;
    


public:
    ////Components
    IComponent*         GetComponent(eCOMPONENT_TYPE _type) const { return (IComponent*)m_arrCom[(UINT)_type]; }

    cCom_Transform*         Transform() { return (cCom_Transform*)m_arrCom[(UINT)eCOMPONENT_TYPE::TRANSFORM]; }

    ICollider2D*        Collider2D() const { return (ICollider2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER2D]; }
    ICollider3D*        Collider3D() const { return (ICollider3D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER3D]; }

    cCom_Animator2D*        Animator2D() const { return (cCom_Animator2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]; }

    cCom_Camera*            Camera() const { return (cCom_Camera*)m_arrCom[(UINT)eCOMPONENT_TYPE::CAMERA]; }

    IRenderer* RenderComponent() const { return (IRenderer*)m_arrCom[(UINT)eCOMPONENT_TYPE::RENDERER]; }

    cScriptHolder*      ScriptHolder() const { return (cScriptHolder*)m_arrCom[(UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER]; }
    cCom_Light2D* Light2D() const { return (cCom_Light2D*)(m_arrCom[(UINT)eCOMPONENT_TYPE::LIGHT2D]); }
};

inline void cGameObject::DestroyRecursive()
{
    m_bDestroy = true;

    size_t size = m_vecChild.size();
    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->DestroyRecursive();
    }
}

inline void cGameObject::SetLifeSpan(float _fLifeSpan)
{
    //이미 수명이 설정되었을 경우에는 retrurn
    if (m_fLifeSpan != FLT_MAX_NEGATIVE)
        return;

    m_fLifeSpan = _fLifeSpan;
}


inline void cGameObject::SetMtrlScalarParam_IntFlag(eMTRLDATA_PARAM_SCALAR _intParam, INT32 _iFlag, bool _bOnOff)
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


inline int cGameObject::GetMtrlScalarParam_Int(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline float cGameObject::GetMtrlScalarParam_Float(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline Vec2 cGameObject::GetMtrlScalarParam_Vec2(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline const Vec4& cGameObject::GetMtrlScalarParam_Vec4(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline const MATRIX& cGameObject::GetMtrlScalarParam_Matrix(eMTRLDATA_PARAM_SCALAR _Param) const
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


inline void cGameObject::YSort(float _MaxZ)
{
    //부모일 경우 자신의 Y값을 깊이값에 반영
    //자식일 경우 부모의 깊이값 + 자신의 상대값을 반영
    enum xyz { x, y, z };
    if (IsMaster())
        m_MtrlScalarData.MAT_1.m[3][z] = _MaxZ + (m_MtrlScalarData.MAT_1.m[3][y] / g_GlobalVal.v2Res.y);
    else
        m_MtrlScalarData.MAT_1.m[3][z] = GetParent()->GetMtrlScalarParam_Matrix(MTRL_SCALAR_MAT_WORLD).m[3][z] + Transform()->GetRelativePos().z;
        
}   


