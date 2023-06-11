#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
public:
    CPrefab();
    virtual ~CPrefab();
    CLONE_DISABLE(CPrefab)

private:
    CGameObject* m_pPrefab;
public:
    //SaveMode로 생성시에는 파일을 제거하지 않음.
    void         RegisterPrefab(CGameObject* _pPrefab, bool _bIsSaveMode = false);

private:
    //SaveMode로 생성시에는 파일을 제거하지 않음.
    bool         m_bSaveMode;
public:
    CGameObject* Instantiate();

public:
    virtual bool Save(const std::filesystem::path& _fileName) override;
    virtual bool Load(const std::filesystem::path& _fileName) override;

    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

private:
    virtual void BindData() override {};
};

