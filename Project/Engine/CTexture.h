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

public:
    //returns 0, 0 if no texture registered.
    Vec2 GetTextureSize();

private:
    virtual int Load(const wstring& _strFilePath) override;
public:
    virtual int Save(const wstring& _strRelativePath) override;



public:
    // _PipelineStage : eSHADER_PIPELINE_STAGE_FLAG
    void UpdateData(int _iRegisterNum, UINT8 _PipelineStage);

private:
    virtual void UpdateData() override;

public:
    CTexture();
    ~CTexture();
};

inline Vec2 CTexture::GetTextureSize()
{
    const DirectX::TexMetadata meta = m_Image.GetMetadata();
    return Vec2((float)meta.width, (float)meta.height);
}