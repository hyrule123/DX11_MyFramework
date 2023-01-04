#pragma once
#include "CShader.h"

enum class eSHADER_LOADTYPE
{
    eSHADER_NOT_LOADED,
    eSHADER_RUNTIME,    //런타임에 컴파일한 쉐이더
    eSHADER_INCLUDE     //헤더를 포함시켜 컴파일한 쉐이더
};

enum eSHADERTYPE
{
    eSHADERTYPE_VERTEX,
    eSHADERTYPE_HULL,
    eSHADERTYPE_DOMAIN,
    eSHADERTYPE_GEOMETRY,
    eSHADERTYPE_PIXEL,
    eSHADERTYPE_END
};

struct tShaderLoadData
{
    eSHADER_LOADTYPE    LoadType;
    ComPtr<ID3DBlob>    Blob;
    void*               pByteCode;
    size_t              ByteCodeSize;
};

class CGraphicsShader :
    public CShader
{
private:
    //지정된 각 쉐이더별로 어떤 방식으로 로드했는지를 저장.
    tShaderLoadData                 m_ShaderData[eSHADERTYPE_END];

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;

    UINT8                           m_ePIPELINE_STAGE_Flag;
    eRS_TYPE                        m_RSType;       //Rasterizer Type

    //RS_TYPE;
    //BS_TYPE;
    //DS_TYPE;

private:


public:
    void CreateInputLayout();
    void CreateShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize, eSHADERTYPE _ShaderType);
    void CreateShader(const wstring& _strFileName, const string& _strFuncName, eSHADERTYPE _ShaderType);
    //void CreateVertexShader(const wstring& _strFileName, const string& _strFuncName);
    //void CreatePixelShader(const wstring& _strFileName, const string& _strFuncName);
    //void CreateVertexShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize);
    //void CreatePixelShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void AddPipeLineStage(UINT8 _eSHADER_PIPELINE_STAGE) { m_ePIPELINE_STAGE_Flag |= _eSHADER_PIPELINE_STAGE; }
    void SetPipeLineStage(UINT8 _eSHADER_PIPELINE_STAGE) { m_ePIPELINE_STAGE_Flag = _eSHADER_PIPELINE_STAGE; }
    void SetRasterizerState(eRS_TYPE _eRS_TYPE) { m_RSType = _eRS_TYPE; }

    virtual void UpdateData() override;
    

public:
    CGraphicsShader();
    ~CGraphicsShader();
};

