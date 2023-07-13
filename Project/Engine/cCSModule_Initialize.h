#pragma once
#include "cShderModule.h"

class cStructBuffer;

class cCSModule_Initialize :
    public cShderModule
{
public:
    cCSModule_Initialize();
    virtual ~cCSModule_Initialize();

public:
    virtual bool Init() override;
    virtual bool BindData() override;
    virtual void UnBind() override;

private:
    std::unique_ptr<cStructBuffer> m_pSBuffer_InitSetting;
};

