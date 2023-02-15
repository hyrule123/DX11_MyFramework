#pragma once
#include "CRes.h"

enum class eSHADER_LOADTYPE
{
    eSHADER_NOT_LOADED,
    eSHADER_RUNTIME,    //��Ÿ�ӿ� �������� ���̴�
    eSHADER_INCLUDE     //����� ���Խ��� �������� ���̴�
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

