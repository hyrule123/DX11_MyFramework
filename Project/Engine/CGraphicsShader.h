#pragma once
#include "CShader.h"



enum eSHADERTYPE
{
    eSHADERTYPE_VERTEX,
    eSHADERTYPE_HULL,
    eSHADERTYPE_DOMAIN,
    eSHADERTYPE_GEOMETRY,
    eSHADERTYPE_PIXEL,
    eSHADERTYPE_END
};



class CGraphicsShader :
    public CShader
{
private:
    //������ �� ���̴����� � ������� �ε��ߴ����� ����.
    tShaderLoadData                 m_ShaderData[eSHADERTYPE_END];

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;


    //Rasterizer
    eRASTERIZER_TYPE                m_RSType;       //Rasterizer Type
    
    //Output Merger
    ////Depth & Stencil
    eDEPTHSTENCIL_TYPE              m_DSType;      //DepthStencil Type
    ////Blend State
    eBLENDSTATE_TYPE                m_BSType;

    eSHADER_DOMAIN                  m_ShaderDomain;

private:


public://INITIALIZE, Setter
    void CreateInputLayout();
    void CreateShader(void* _pShaderByteCode, size_t _ShaderByteCodeSize, eSHADERTYPE _ShaderType);
    void CreateShader(const wstring& _strFileName, const string& _strFuncName, eSHADERTYPE _ShaderType);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void SetRasterizerState(eRASTERIZER_TYPE _eRS_TYPE) { m_RSType = _eRS_TYPE; }
    void SetDepthStencilState(eDEPTHSTENCIL_TYPE _eDS_TYPE) { m_DSType = _eDS_TYPE; }
    void SetBlendState(eBLENDSTATE_TYPE _eBS_TYPE) { m_BSType = _eBS_TYPE; }
    void SetShaderDomain(eSHADER_DOMAIN _eSD_TYPE) { m_ShaderDomain = _eSD_TYPE; }

    //Getter
    eSHADER_DOMAIN GetShaderDomain() const { return m_ShaderDomain; }

public:
    virtual void BindData() override;
    

public:
    CGraphicsShader();
    ~CGraphicsShader();
};

