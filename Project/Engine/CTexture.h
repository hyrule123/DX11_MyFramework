#pragma once
#include "CRes.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif

class CTexture :
    public CRes
{
private:
    ComPtr<ID3D11Texture2D>             m_Tex2D;    
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    D3D11_TEXTURE2D_DESC                m_Desc;

    ScratchImage                        m_Image;


private:
    virtual int Load(const wstring& _strFilePath) override;
public:
    virtual int Save(const wstring& _strRelativePath) override;



public:
    // _PipelineStage : ePIPELINE_STAGE
    void UpdateData(int _iRegisterNum, UINT8 _PipelineStage);

private:
    virtual void UpdateData() override;

public:
    CTexture();
    ~CTexture();
};

