#pragma once
#include "CRes.h"


class CShader :
    public CRes
{
protected:
    ComPtr<ID3DBlob>    m_ErrBlob;

public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }
private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }

public:
    CShader(eRES_TYPE _eType);
    ~CShader();
};

