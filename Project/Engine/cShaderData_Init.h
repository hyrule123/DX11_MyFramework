#pragma once
#include "cGPUBufferModule.h"

class cStructBuffer;

class cShaderData_Init :
    public cGPUBufferModule
{
public:
    cShaderData_Init();
    virtual ~cShaderData_Init();

public:
    virtual tNumDataCS BindDataCS() override;
    virtual bool BindDataGS() override;
    virtual void UnBind() override;

private:
    std::unique_ptr<cStructBuffer> m_pSBuffer_InitSetting;
};

