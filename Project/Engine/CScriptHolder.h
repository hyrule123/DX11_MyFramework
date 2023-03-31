#pragma once
#include "CComponent.h"

#include "CScript.h"


class CTransform;
class CCamera;
class CMeshRender;
class CCollider;

class CScriptHolder :
    public CComponent
{
public:
    CScriptHolder();
    CScriptHolder(const CScriptHolder& _other);
    virtual ~CScriptHolder();
    CLONE(CScriptHolder);

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

    

public:
    bool AddScript(CScript* _pScript);

    template <typename T>
    T* GetScript();

    CScript* GetScript(std::type_index _typeIdx);

    const vector<CScript*>& GetScripts() const { return m_vecScript; }

    //들고 있는 Script 들에도 Owner 설정이 필요하므로 재정의해서 사용한다.
    virtual void SetOwner(CGameObject* _pOwner) override;

public:
    void BeginColiision(CCollider* _Other);
    void OnCollision(CCollider* _Other);
    void EndCollision(CCollider* _Other);
};

template <typename T>
inline T* CScriptHolder::GetScript()
{
    std::type_index TypeIdx = std::type_index(typeid(T));

    const auto& iter = m_umapScript.find(TypeIdx);

    if (iter != m_umapScript.end())
        return (T*)iter->second;

    //size_t size = m_vecScript.size();
    //for (size_t i = 0; i < size; ++i)
    //{
    //    if (TypeIdx == m_vecScript[i]->GetTypeIndex())
    //        return static_cast<T*>(m_vecScript[i]);
    //}

    return nullptr;
}
