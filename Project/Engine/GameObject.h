#pragma once
#include "Entity.h"

#include "global.h"

#include "Transform.h"

class Component;
class Transform;
class Collider2D;
class MeshRenderer;
class Camera;
class RenderComponent;
class ScriptHolder;
class Script;
class Light2D;
class Tilemap;
class Animator2D;
class Collider3D;

class GameObject :
    public Entity
{
    
public:
    GameObject();

    GameObject(const GameObject& _other);
    CLONE(GameObject);

    virtual ~GameObject();

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
    Transform              m_Transform;
    //Components
    Component*             m_arrCom[(UINT)eCOMPONENT_TYPE::END];
    RenderComponent*       m_RenderCom;
public:
    //Add
    void AddComponent(Component* _Component);

    //EventMgr 전용 함수. 직접 호출 시 에러 발생할 수 있음.
    void RemoveComponent(eCOMPONENT_TYPE _eComType);

    void AddScript(Script* _Script);

private:
    //Hierarchy
    GameObject*            m_Parent;
    vector<GameObject*>    m_vecChild;
public:
    void                SetParent(GameObject* _pObj) { m_Parent = _pObj; }
    GameObject*        GetParent() const { return m_Parent; }

    //Master GameObject만 LevelMgr에서 tick()를 호출한다.
    bool                IsMaster()  const { return (nullptr == m_Parent); }

    void                AddChildGameObj(GameObject* _Object);

    void                RemoveChild(GameObject* _Object);


    //기타
    bool                GetParentWorldMatrix(Matrix& _mat);

    const vector<GameObject*>& GetvecChilds() const { return m_vecChild; }
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

    //Camera에서 호출.
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
    //EventMgr에서 사용. 자식 오브젝트까지 재귀적으로 삭제
    void                    DestroyRecursive();
    bool                    IsDestroyed() const { return m_bDestroy; }

private:
    bool                    m_bStart;

    bool                    m_bDisable;
    bool                    m_bPrevEnable;
    


public:
    ////Components
    Component*         GetComponent(eCOMPONENT_TYPE _type) const { return (Component*)m_arrCom[(UINT)_type]; }

    Transform&         Transform() { return m_Transform; }

    Collider2D*        Collider2D() const { return (Collider2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER2D]; }
    Collider3D*        Collider3D() const { return (Collider3D*)m_arrCom[(UINT)eCOMPONENT_TYPE::COLLIDER3D]; }

    Animator2D*        Animator2D() const { return (Animator2D*)m_arrCom[(UINT)eCOMPONENT_TYPE::ANIMATOR2D]; }

    MeshRenderer*        MeshRenderer() const { return (MeshRenderer*)m_arrCom[(UINT)eCOMPONENT_TYPE::MESH_RENDER]; }
    Tilemap*           Tilemap() const { return (Tilemap*)m_arrCom[(UINT)eCOMPONENT_TYPE::TILEMAP]; }

    Camera*            Camera() const { return (Camera*)m_arrCom[(UINT)eCOMPONENT_TYPE::CAMERA]; }

    RenderComponent*   RenderComponent() const { return m_RenderCom; }

    ScriptHolder*      ScriptHolder() const { return (ScriptHolder*)m_arrCom[(UINT)eCOMPONENT_TYPE::SCRIPT_HOLDER]; }
    Light2D* Light2D() const { return (Light2D*)(m_arrCom[(UINT)eCOMPONENT_TYPE::LIGHT2D]); }
};

inline void GameObject::DestroyRecursive()
{
    m_bDestroy = true;

    size_t size = m_vecChild.size();
    for (size_t i = 0; i < size; i++)
    {
        m_vecChild[i]->DestroyRecursive();
    }
}

inline void GameObject::SetLifeSpan(float _fLifeSpan)
{
    //이미 수명이 설정되었을 경우에는 retrurn
    if (m_fLifeSpan != FLT_MAX_NEGATIVE)
        return;

    m_fLifeSpan = _fLifeSpan;
}


inline void GameObject::SetMtrlScalarParam_IntFlag(eMTRLDATA_PARAM_SCALAR _intParam, INT32 _iFlag, bool _bOnOff)
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


inline int GameObject::GetMtrlScalarParam_Int(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline float GameObject::GetMtrlScalarParam_Float(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline Vec2 GameObject::GetMtrlScalarParam_Vec2(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline const Vec4& GameObject::GetMtrlScalarParam_Vec4(eMTRLDATA_PARAM_SCALAR _Param) const
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

inline const MATRIX& GameObject::GetMtrlScalarParam_Matrix(eMTRLDATA_PARAM_SCALAR _Param) const
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


inline void GameObject::YSort(float _MaxZ)
{
    //부모일 경우 자신의 Y값을 깊이값에 반영
    //자식일 경우 부모의 깊이값 + 자신의 상대값을 반영
    enum xyz { x, y, z };
    if (IsMaster())
        m_MtrlScalarData.MAT_1.m[3][z] = _MaxZ + (m_MtrlScalarData.MAT_1.m[3][y] / g_GlobalVal.v2Res.y);
    else
        m_MtrlScalarData.MAT_1.m[3][z] = GetParent()->GetMtrlScalarParam_Matrix(MTRL_SCALAR_MAT_WORLD).m[3][z] + Transform().GetRelativePos().z;
        
}


