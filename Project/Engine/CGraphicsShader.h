#pragma once
#include "CShader.h"

class CGraphicsShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                m_VSBlob;
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;    

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;

    UINT8                   m_ePIPELINE_STAGE_Flag;


    //RS_TYPE;
    //BS_TYPE;
    //DS_TYPE;


public:
    void CreateVertexShader(const wstring& _strFileName, const string& _strFuncName);
    void CreatePixelShader(const wstring& _strFileName, const string& _strFuncName);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void AddPipeLineStage(UINT8 _ePIPELINE_STAGE_ENUM) { m_ePIPELINE_STAGE_Flag |= _ePIPELINE_STAGE_ENUM; }
    void SetPipeLineStage(UINT8 _ePIPELINE_STAGE_ENUM) { m_ePIPELINE_STAGE_Flag = _ePIPELINE_STAGE_ENUM; }

    virtual void UpdateData() override;


public:
    CGraphicsShader();
    ~CGraphicsShader();
};

