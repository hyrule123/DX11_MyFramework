#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
    CGameObject* m_pPrefab;
public:
    void         RegisterPrefab(CGameObject* _pPrefab);
    CGameObject* Instantiate();

private:
    virtual bool Load(const std::filesystem::path& _path) override { return false; }

public:
    virtual bool Save(const std::filesystem::path& _path) override { return false; }

private:
    virtual void BindData() override {};

public:
    CPrefab();
    virtual ~CPrefab();

    CLONE_DISABLE(CPrefab)
};

