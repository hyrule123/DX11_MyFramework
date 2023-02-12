#pragma once
#include "CRes.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_Debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif

class CTexture :
    public CRes
{
public:
    CTexture();
    ~CTexture();

private:
    ComPtr<ID3D11Texture2D>             m_Tex2D;    

    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    //DSV�� �ٸ� ���� �ߺ��ؼ� ���ε��� �� ����.
    //DSV�� ����ϰ��� �� ���� ������ Dsv �뵵�θ� ����ؾ� ��. 
    ComPtr<ID3D11DepthStencilView>      m_DSV;

    D3D11_TEXTURE2D_DESC                m_Desc;

    ScratchImage                        m_Image;

    //��ǻƮ���̴��� ���ε����� ��� �ش� ��ǻƮ���̴� �������� ��ȣ�� ����. 
    //���߿� UnBind�� �� ����.
    eCURRENT_BOUND_VIEW           m_eCurBoundView;
    UINT                          m_flagCurBoundPipeline;
    int                           m_iCurBoundRegister;

                                

public:
    //returns 0, 0 if no texture registered.
    Vec2 GetSize();
    float GetWidth() { return (float)m_Desc.Width; }
    float GetHeight() { return (float)m_Desc.Height; }


    ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
    ComPtr<ID3D11RenderTargetView>	    GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView>	    GetDSV() { return m_DSV; }
    ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }

private:
    virtual int Load(const wstring& _strFilePath) override;
public:
    virtual int Save(const wstring& _strRelativePath) override;
    
public:
    int Create(UINT _uWidth, UINT _uHeight, DXGI_FORMAT _pixelFormat, UINT _D3D11_BIND_FLAG, D3D11_USAGE _Usage);
    int Create(ComPtr<ID3D11Texture2D> _pTex2D);

public:
    int CreateView();

public:
    // _PipelineStage : eSHADER_PIPELINE_STAGE::FLAG
    void BindData_SRV(int _iRegisterNum, UINT _eSHADER_PIPELINE_STAGE);

    //Bind Texture data to Compute Shader 
    void BindData_UAV(int _iRegisterNum);

    void UnBind();

private://������ BindData()�� ����ؼ� ���ε��Ѵ�.
    virtual void BindData() override {};
    


};

inline Vec2 CTexture::GetSize()
{
    return Vec2(m_Desc.Width, m_Desc.Height);
}