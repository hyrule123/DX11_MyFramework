#pragma once
#include "CShader.h"

#include "global.h"

//참고 - CEntity의 'Name' 변수에 쉐이더의 BaseName을 저장함.

class CGraphicsShader :
    public CShader
{
public:
    CGraphicsShader();
    virtual ~CGraphicsShader();

public:
    //현재 CSO 파일만 로드 가능하므로 참고
    //다른 
    virtual bool Save(const std::filesystem::path& _fileName) override;
    
    virtual bool Load(const std::filesystem::path& _fileName) override;

    virtual bool SaveJson(Json::Value* _jsonVal) override;
    virtual bool LoadJson(Json::Value* _jsonVal) override;

private:

    //지정된 각 쉐이더별로 어떤 방식으로 로드했는지를 저장.
    tShaderLoadData                 m_ShaderData[(int)eSHADER_TYPE::END];

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;


    //Rasterizer
    eRASTERIZER_TYPE                m_eRSType;       //Rasterizer Type
    
    //Output Merger
    ////Depth & Stencil
    eDEPTHSTENCIL_TYPE              m_eDSType;      //DepthStencil Type
    ////Blend State
    eBLENDSTATE_TYPE                m_eBSType;

    eSHADER_DOMAIN                  m_eShaderDomain;

private:


public://INITIALIZE, Setter
    void CreateDefaultInputLayout();
    void CreateShader(char* _pShaderByteCode, size_t _ShaderByteCodeSize, eSHADER_TYPE _ShaderType, eSHADER_LOADTYPE _LoadType);
    void CreateShader(const wstring& _strFileName, const string& _strFuncName, eSHADER_TYPE _ShaderType);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void SetRasterizerState(eRASTERIZER_TYPE _eRS_TYPE) { m_eRSType = _eRS_TYPE; }
    void SetDepthStencilState(eDEPTHSTENCIL_TYPE _eDS_TYPE) { m_eDSType = _eDS_TYPE; }
    void SetBlendState(eBLENDSTATE_TYPE _eBS_TYPE) { m_eBSType = _eBS_TYPE; }
    void SetShaderDomain(eSHADER_DOMAIN _eSD_TYPE) { m_eShaderDomain = _eSD_TYPE; }

    //Getter
    eSHADER_DOMAIN GetShaderDomain() const { return m_eShaderDomain; }

public:
    virtual void BindData() override;
};

