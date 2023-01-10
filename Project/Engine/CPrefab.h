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
    virtual int Load(const wstring& _strFilePath) override { return 0; }

public:
    virtual int Save(const wstring& _strRelativePath) override { return 0; }

private:
    virtual void UpdateData() override {};

public:
    CPrefab();
    virtual ~CPrefab();

    CLONE_DISABLE(CPrefab)
};

