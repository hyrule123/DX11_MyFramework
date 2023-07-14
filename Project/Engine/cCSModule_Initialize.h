#pragma once
#include "cShaderModule.h"

class cStructBuffer;

class cCSModule_Initialize :
    public cShaderModule
{
public:
    cCSModule_Initialize();
    virtual ~cCSModule_Initialize();

public:
    virtual bool Init() override;
    virtual bool BindDataCS() override;
    virtual bool BindDataGS() override {}
    virtual void UnBind() override;

private:
    std::unique_ptr<cStructBuffer> m_pSBuffer_InitSetting;
};

