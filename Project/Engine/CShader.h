#pragma once
#include "CRes.h"

enum class eSHADER_LOADTYPE
{
    NOT_LOADED,
    RUNTIME_COMPILED,    //런타임에 컴파일한 쉐이더
    BYTE_CODE_INCLUDED,     //헤더를 포함시켜 컴파일한 쉐이더
    BYTE_CODE_FROM_FILE
};

struct tShaderLoadData
{
    eSHADER_LOADTYPE    LoadType;
    ComPtr<ID3DBlob>    Blob;
    char* pByteCode;
    size_t              ByteCodeSize;

    ~tShaderLoadData()
    {
        if (LoadType == eSHADER_LOADTYPE::BYTE_CODE_FROM_FILE)
        {
            delete[] pByteCode; 
        }
    }
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
    virtual bool Save(const std::filesystem::path& _path) { return false; }
private:
    virtual bool Load(const std::filesystem::path& _path) { return false; }


};

