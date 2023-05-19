#pragma once
#include "CComponent.h"


class CTransform;
class CCamera;
class CMeshRender;
class CCollider;
class CFSM_Mgr;
class CFSM;

class CScriptHolder :
    public CComponent
{
public:
    CScriptHolder();

    CScriptHolder(const CScriptHolder& _other);
    CLONE(CScriptHolder);

    virtual ~CScriptHolder();

public:
    virtual bool SaveJson(Json::Value* _jVal);
    virtual bool LoadJson(Json::Value* _jVal);

public:
    virtual void init() final;
    virtual void start() final;
    virtual void tick() final;
    virtual void finaltick() final {}
    virtual void cleanup() final {}

    CFSM* Transition(UINT _eState);

private:
    //같은 스크립트에 대해 두 개의 컨테이너가 들고 있으므로 반드시 추가/제거할 때 주의할것.
    vector<CScript*> m_vecScript;
    unordered_map<std::type_index, CScript*> m_umapScript;

    CFSM_Mgr* m_pFStateMgr;

public:
    bool AddScript(CScript* _pScript);

    template <typename T>
    T* GetScript();
    CScript* GetScript(std::type_index _t);
    const vector<CScript*>& GetScripts() const { return m_vecScript; }

    void RegisterFStateMgr(CFSM_Mgr* _pFStateMgr);
    CFSM_Mgr* GetFStateMgr() const { return m_pFStateMgr; }
    
public:
    void BeginColiision(CCollider* _Other, const Vec3& _v3HitPoint);
    void OnCollision(CCollider* _Other, const Vec3& _v3HitPoint);
    void EndCollision(CCollider* _Other);
};

template <typename T>
inline T* CScriptHolder::GetScript()
{
    return (T*)GetScript(std::type_index(typeid(T)));
}

inline CScript* CScriptHolder::GetScript(std::type_index _t)
{
    const auto& iter = m_umapScript.find(_t);

    if (iter != m_umapScript.end())
        return iter->second;

    return nullptr;
}

inline void CScriptHolder::RegisterFStateMgr(CFSM_Mgr* _pFStateMgr)
{
    assert(nullptr == m_pFStateMgr);
    m_pFStateMgr = _pFStateMgr;
}

