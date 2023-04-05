#pragma once
#include "CComponent.h"


class CTransform;
class CCamera;
class CMeshRender;
class CCollider;
class CFStateMgr;
class CFState;

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
    virtual void tick() final;
    virtual void finaltick() final {}
    virtual void cleanup() final {}

private:
    //같은 스크립트에 대해 두 개의 컨테이너가 들고 있으므로 반드시 추가/제거할 때 주의할것.
    vector<CScript*> m_vecScript;
    unordered_map<std::type_index, CScript*> m_umapScript;

    CFStateMgr* m_pFStateMgr;

public:
    bool AddScript(CScript* _pScript);

    template <typename T>
    T* GetScript();

    CScript* GetScript(std::type_index _typeIdx);

    const vector<CScript*>& GetScripts() const { return m_vecScript; }

    //들고 있는 Script 들에도 Owner 설정이 필요하므로 재정의해서 사용한다.
    virtual void SetOwner(CGameObject* _pOwner) override;

    void SetFStateMgr(CFStateMgr* _pFStateMgr) { m_pFStateMgr = _pFStateMgr; }
    CFStateMgr* GetFStateMgr() const { return m_pFStateMgr; }

    CFState* Transition(UINT _eState);
    


public:
    void BeginColiision(CCollider* _Other, const Vec3& _v3HitPoint);
    void OnCollision(CCollider* _Other, const Vec3& _v3HitPoint);
    void EndCollision(CCollider* _Other);
};

template <typename T>
inline T* CScriptHolder::GetScript()
{
    const auto& iter = m_umapScript.find(std::type_index(typeid(T)));

    if (iter != m_umapScript.end())
        return (T*)iter->second;

    return nullptr;
}
