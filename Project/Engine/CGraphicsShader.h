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
    //virtual bool Save(const std::filesystem::path& _fileName) override;
    
    //virtual bool Load(const std::filesystem::path& _fileName) override;

    virtual bool SaveJson(Json::Value* _jsonVal) override;
    virtual bool LoadJson(Json::Value* _jsonVal) override;

private:
    //지정된 각 쉐이더별로 어떤 방식으로 로드했는지를 저장.
    ComPtr<ID3DBlob>                m_arr_pShaderData[(int)eSHADER_TYPE::END];

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_InputLayout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;


    //Rasterizer
    eRASTERIZER_TYPE                m_eRSType;       //Rasterizer Type
    
    //Output Merger
    ////Depth & Stencil
    eDEPTH_STENCIL_TYPE              m_eDSType;      //DepthStencil Type
    ////Blend State
    eBLEND_STATE_TYPE                m_eBSType;

    eSHADER_DOMAIN                  m_eShaderDomain;

    bool                            m_bUseInstancing;

private:


public://INITIALIZE, Setter
    HRESULT CreateDefaultInputLayout();

    HRESULT CreateShader(char* _pShaderByteCode, size_t _ShaderByteCodeSize, eSHADER_TYPE _ShaderType);
    HRESULT CreateShader(const wstring& _strFileName, const string& _strFuncName, eSHADER_TYPE _ShaderType);
    HRESULT CreateShader(ComPtr<ID3DBlob> _pBlob, eSHADER_TYPE _ShaderType);
private:
    HRESULT CreateShader(eSHADER_TYPE _ShaderType);

public:

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void SetRasterizerState(eRASTERIZER_TYPE _eRS_TYPE) { m_eRSType = _eRS_TYPE; }

    void SetDepthStencilState(eDEPTH_STENCIL_TYPE _eDS_TYPE) { m_eDSType = _eDS_TYPE; }

    void SetBlendState(eBLEND_STATE_TYPE _eBS_TYPE) { m_eBSType = _eBS_TYPE; }

    void SetShaderDomain(eSHADER_DOMAIN _eSD_TYPE) { m_eShaderDomain = _eSD_TYPE; }
    eSHADER_DOMAIN GetShaderDomain() const { return m_eShaderDomain; }

    void SetUseInstancing(bool _bAble) { m_bUseInstancing = _bAble; }
    bool IsUseInstancing() const { return m_bUseInstancing; }


    

public:
    virtual void BindData() override;
};


inline HRESULT CGraphicsShader::CreateShader(ComPtr<ID3DBlob> _pBlob, eSHADER_TYPE _eShaderType)
{
    //기존의 blob과 데이터를 교체하고 
    m_arr_pShaderData[(int)_eShaderType].Swap(_pBlob);

    //쉐이더를 생성한다.
    return CreateShader(_eShaderType);
}
