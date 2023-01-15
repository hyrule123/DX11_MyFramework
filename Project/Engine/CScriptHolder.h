#pragma once
#include "CComponent.h"

class CScript;
class CTransform;
class CCamera;
class CMeshRender;

class CScriptHolder :
    public CComponent
{
private:
    vector<CScript*> m_vecScript;

public:
    bool AddScript(CScript* _pScript);
    template <typename T>
    T* GetScript();
    const vector<CScript*>& GetScripts() const { return m_vecScript; }

    //들고 있는 Script 들에도 Owner 설정이 필요하므로 재정의해서 사용한다.
    virtual void SetOwner(CGameObject* _pOwner) override;

public:
    virtual void init() final;
    virtual void tick() final;
    virtual void finaltick() final {}

public:
    CScriptHolder();
    CScriptHolder(const CScriptHolder& _other);
    virtual ~CScriptHolder();

    CLONE(CScriptHolder)
};

template <typename T>
inline T* CScriptHolder::GetScript()
{
    size_t size = m_vecScript.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (std::type_index(typeid(T)) == m_vecScript[i]->GetTypeIndex())
            return static_cast<T*>(m_vecScript[i]);
    }

    return nullptr;
}
