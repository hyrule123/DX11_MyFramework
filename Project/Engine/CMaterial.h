#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial :
    public CRes
{
public:
    CMaterial();
    ~CMaterial();

    CLONE(CMaterial)

private:
    Ptr<CGraphicsShader>    m_pShader;

    tMtrlData               m_MtrlData;

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

private:
    bool IsTexture(eMTRLDATA_PARAM_TEX _Idx);

};

inline Ptr<CTexture> CMaterial::GetTexture(eMTRLDATA_PARAM_TEX _texIdx) const
{
    assert(_texIdx < eMTRLDATA_PARAM_TEX::_END);
    return m_arrTex[(int)_texIdx];
}

inline bool CMaterial::IsTexture(eMTRLDATA_PARAM_TEX _Idx)
{
    switch (_Idx)
    {
    case eMTRLDATA_PARAM_TEX::_0: return (bool)m_MtrlData.bTEX_0;
    case eMTRLDATA_PARAM_TEX::_1: return (bool)m_MtrlData.bTEX_1;
    case eMTRLDATA_PARAM_TEX::_2: return (bool)m_MtrlData.bTEX_2;
    case eMTRLDATA_PARAM_TEX::_3: return (bool)m_MtrlData.bTEX_3;
    case eMTRLDATA_PARAM_TEX::_4: return (bool)m_MtrlData.bTEX_4;
    case eMTRLDATA_PARAM_TEX::_5: return (bool)m_MtrlData.bTEX_5;
    case eMTRLDATA_PARAM_TEX::_6: return (bool)m_MtrlData.bTEX_6;
    case eMTRLDATA_PARAM_TEX::_7: return (bool)m_MtrlData.bTEX_7;
    case eMTRLDATA_PARAM_TEX::_END: break;
    default: break;
    }

    return false;
}