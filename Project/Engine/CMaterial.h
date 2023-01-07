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


    Ptr<CTexture>           m_arrTex[TEX_END];


public:
    void SetScalarParam(eSCALAR_PARAM _Param, void* _Src);
    void SetTexParam(eTEX_PARAM _Param, Ptr<CTexture> _Tex);


    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }
    Ptr<CGraphicsShader> GetShader() const { return m_pShader; }
    virtual void UpdateData() override;





private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }

public:
    CMaterial();
    ~CMaterial();
};

