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

