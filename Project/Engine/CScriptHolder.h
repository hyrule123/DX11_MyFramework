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
    map<std::string, CScript*> m_mapScript;

public:
    bool AddScript(CScript* _pScript);
    CScript* FindScript(const string& _strName);
    const vector<CScript*>& GetScripts() const { return m_vecScript; }

public:
    void BeginColiision(CCollider* _Other, const Vec3& _v3HitPoint);
    void OnCollision(CCollider* _Other, const Vec3& _v3HitPoint);
    void EndCollision(CCollider* _Other);
};


inline CScript* CScriptHolder::FindScript(const string& _strName)
{
    const auto& iter = m_mapScript.find(_strName);

    if (iter != m_mapScript.end())
        return iter->second;

    return nullptr;
}

