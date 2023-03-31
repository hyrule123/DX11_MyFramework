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

public:
    virtual bool Save(const std::filesystem::path& _fileName) override;
    virtual bool Load(const std::filesystem::path& _fileName) override;

    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

private:
    virtual void BindData() override {};

public:
    CPrefab();
    virtual ~CPrefab();

    CLONE_DISABLE(CPrefab)
};

