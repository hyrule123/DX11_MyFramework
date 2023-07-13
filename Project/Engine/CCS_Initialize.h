#pragma once
#include "C_ComputeShader.h"

class cStructBuffer;

class CCS_Initialize :
    public C_ComputeShader
{
public:
    CCS_Initialize();
    virtual ~CCS_Initialize();

public:
    virtual bool Load(std::filesystem::path const& _FilePath) override;
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    std::unique_ptr<cStructBuffer> m_pSBuffer_InitSetting;

};

