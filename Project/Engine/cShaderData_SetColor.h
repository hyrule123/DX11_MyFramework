#pragma once
#include "cShaderDataModule.h"

#include "Ptr.h"

class cTexture;
class cStructBuffer;
class cShaderData_SetColor :
    public cShaderDataModule
{
public:
    cShaderData_SetColor();
    virtual ~cShaderData_SetColor();

public:
    virtual tNumDataCS BindDataCS() override;
    virtual bool BindDataGS() override;
    virtual void UnBind() override;

private:
    Ptr<cTexture>   m_OutTex;
    std::unique_ptr<cStructBuffer>   m_StructBufferTest;
    Vec4            m_vecSBuffer[1280];

public:
    void SetTargetTexture(Ptr<cTexture> _Tex) { m_OutTex = _Tex; }
};
