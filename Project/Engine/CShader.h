#pragma once
#include "CRes.h"

enum class eSHADER_LOADTYPE
{
    NOT_LOADED,
    RUNTIME_COMPILED,    //런타임에 컴파일한 쉐이더
    BYTE_CODE     //헤더를 포함시켜 컴파일한 쉐이더
};

struct tShaderLoadData
{
    eSHADER_LOADTYPE    LoadType;
    ComPtr<ID3DBlob>    Blob;
    void* pByteCode;
    size_t              ByteCodeSize;
};

class CShader :
    public CRes
{
public:
    CShader(eRES_TYPE _eType);
    virtual ~CShader();

protected:
    ComPtr<ID3DBlob>    m_ErrBlob;

public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }
private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }


};

