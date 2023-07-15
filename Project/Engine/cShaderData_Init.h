#pragma once
#include "cShaderDataModule.h"

class cStructBuffer;

class cShaderData_Init :
    public cShaderDataModule
{
public:
    cShaderData_Init();
    virtual ~cShaderData_Init();

public:
    virtual bool Init() override;
    virtual tNumData BindDataCS() override;
    virtual bool BindDataGS() override {}
    virtual void UnBind() override;

private:
    std::unique_ptr<cStructBuffer> m_pSBuffer_InitSetting;
};

