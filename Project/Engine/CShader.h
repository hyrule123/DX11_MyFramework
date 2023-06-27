#pragma once
#include "CRes.h"

enum class eSHADER_LOADTYPE
{
    NOT_LOADED,
    RUNTIME_COMPILED,    //런타임에 컴파일한 쉐이더
    BYTE_CODE_INCLUDED,     //헤더를 포함시켜 컴파일한 쉐이더
    BYTE_CODE_FROM_FILE
};

namespace SHADER_NAME_VERSION
{
    constexpr const char* GS[(int)define_Shader::eSHADER_TYPE::END]
        = {
        "vs_5_0",
        "hs_5_0",
        "ds_5_0",
        "gs_5_0",
        "ps_5_0"
    };
    constexpr const char* CS = "cs_5_0";
}

class CShader :
    public CRes
{
public:
    CShader(eRES_TYPE _eType);
    virtual ~CShader();

protected:
    ComPtr<ID3DBlob>    m_ErrBlob;

public:
    //virtual bool Save(const std::filesystem::path& _fileName) { return false; }
    //virtual bool Load(const std::filesystem::path& _fileName) { return false; }


};

