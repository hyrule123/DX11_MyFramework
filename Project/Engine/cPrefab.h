#pragma once
#include "IRes.h"




class cGameObject;
class CPrefab :
    public IRes
{
public:
    CPrefab();
    virtual ~CPrefab();
    CLONE_DISABLE(CPrefab)

private:
    cGameObject* m_pPrefab;
public:
    //SaveMode로 생성시에는 파일을 제거하지 않음.
    void         RegisterPrefab(cGameObject* _pPrefab, bool _bIsSaveMode = false);
    bool         IsAvailable() const { return nullptr != m_pPrefab; }

private:
    //SaveMode로 생성시에는 파일을 제거하지 않음.
    bool         m_bSaveMode;
public:
    cGameObject* Instantiate();

public:
    virtual bool Save(const std::filesystem::path& _fileName) override;
    virtual bool Load(const std::filesystem::path& _fileName) override;

    virtual bool SaveJson(Json::Value* _pJVal) override;
    virtual bool LoadJson(Json::Value* _pJVal) override;

};

