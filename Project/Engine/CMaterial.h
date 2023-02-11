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

    tMtrlData              m_Const;

    Ptr<CTexture>           m_arrTex[(int)eMTRLDATA_PARAM_TEX::_END];


public:
    //Inline Setter
    void SetScalarParam(eMTRLDATA_PARAM_SCALAR _Param, const void* _Src);
    void SetTexParam(eMTRLDATA_PARAM_TEX _Param, Ptr<CTexture> _Tex);
    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }

    //Inline Getter
    Ptr<CGraphicsShader> GetShader() const { return m_pShader; }
    Ptr<CTexture> GetTexture(eMTRLDATA_PARAM_TEX _texIdx = eMTRLDATA_PARAM_TEX::_0) const;


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

inline Ptr<CTexture> CMaterial::GetTexture(eMTRLDATA_PARAM_TEX _texIdx) const
{
    assert(_texIdx < eMTRLDATA_PARAM_TEX::_END);
    return m_arrTex[(int)_texIdx];
}
