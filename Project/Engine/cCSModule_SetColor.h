#pragma once
#include "cShaderModule.h"

#include "Ptr.h"

class cTexture;
class cStructBuffer;
class cCSModule_SetColor :
    public cShaderModule
{
public:
    cCSModule_SetColor();
    virtual ~cCSModule_SetColor();

public:
    virtual bool Init() override;
    virtual bool BindData() override;
    virtual void UnBind() override;

private:
    Ptr<cTexture>   m_OutTex;
    std::unique_ptr<cStructBuffer>   m_StructBufferTest;
    Vec4            m_vecSBuffer[1280];

public:
    void SetTargetTexture(Ptr<cTexture> _Tex) { m_OutTex = _Tex; }
    void SetColor(const Vec3& _RGB) { GetOwner()->SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::VEC4_0, &_RGB); }
};
