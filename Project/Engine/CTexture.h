#pragma once
#include "CRes.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_Debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex_Release")
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

    //DSV는 다른 뷰들과 중복해서 바인딩할 수 없음.
    //DSV로 사용하고자 할 때는 무조건 Dsv 용도로만 사용해야 함. 
    ComPtr<ID3D11DepthStencilView>      m_DSV;

    D3D11_TEXTURE2D_DESC                m_Desc;

    ScratchImage                        m_Image;

    //컴퓨트쉐이더와 바인딩했을 경우 해당 컴퓨트쉐이더 레지스터 번호를 저장. 
    //나중에 UnBind할 때 사용됨.
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

private://별도의 BindData()를 사용해서 바인딩한다.
    virtual void BindData() override {};
    


};

inline Vec2 CTexture::GetSize()
{
    return Vec2(m_Desc.Width, m_Desc.Height);
}