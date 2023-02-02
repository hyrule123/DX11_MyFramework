#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial :
    public CRes
{
private:
    Ptr<CGraphicsShader>    m_pShader;

    tMtrlConst              m_Const;

    Ptr<CTexture>           m_arrTex[eTEX_END];


public:
    //Inline Setter
    void SetScalarParam(eSCALAR_PARAM _Param, const void* _Src);
    void SetTexParam(eTEX_PARAM _Param, Ptr<CTexture> _Tex);
    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }

    //Inline Getter
    Ptr<CGraphicsShader> GetShader() const { return m_pShader; }
    Ptr<CTexture> GetTexture(eTEX_PARAM _texIdx = eTEX_0) const;


private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }

    virtual void BindData() override;

public:
    CMaterial();
    ~CMaterial();

    CLONE(CMaterial)
};

inline Ptr<CTexture> CMaterial::GetTexture(eTEX_PARAM _texIdx) const
{
    assert(0 <= _texIdx && _texIdx < eTEX_END);
    return m_arrTex[_texIdx];
}
