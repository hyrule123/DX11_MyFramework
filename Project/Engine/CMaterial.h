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

    //����� ������۸� ��� ���������� �ܰ迡 �����Ұ������� ����.
    UINT8                   m_flag_ePIPELINE_STAGE;

    Ptr<CTexture>           m_arrTex[TEX_END];


public:
    void SetScalarParam(eSCALAR_PARAM _Param, void* _Src);
    void SetCBufferPipelineStage(UINT8 _flag_ePIPELINE_STATUS) { m_flag_ePIPELINE_STAGE = _flag_ePIPELINE_STATUS; }
    void AddCBufferPipelineStage(ePIPELINE_STAGE _PipelineStage) { m_flag_ePIPELINE_STAGE |= (UINT8)_PipelineStage; }
    void SetTexParam(eTEX_PARAM _Param, const Ptr<CTexture>& _Tex);


    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }
    virtual void UpdateData() override;





private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }

public:
    CMaterial();
    ~CMaterial();
};

