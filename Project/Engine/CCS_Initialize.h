#pragma once
#include "CComputeShader.h"

class CStructBuffer;

class CCS_Initialize :
    public CComputeShader
{
public:
    CCS_Initialize();
    virtual ~CCS_Initialize();

public:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    std::unique_ptr<CStructBuffer> m_pSBuffer_InitSetting;

};

