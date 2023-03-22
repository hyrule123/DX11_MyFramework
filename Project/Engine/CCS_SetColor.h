#pragma once
#include "CComputeShader.h"

#include "ptr.h"



class CTexture;
class CStructBuffer;

class CCS_SetColor :
    public CComputeShader
{
public:
    CCS_SetColor();
    virtual ~CCS_SetColor();

protected:
    virtual bool BindDataCS() override;
    virtual void UnBindCS() override;

private:
    Ptr<CTexture>   m_OutTex;
    CStructBuffer*   m_StructBufferTest;
    Vec4            m_vecSBuffer[1280];

public:
    void SetTargetTexture(Ptr<CTexture> _Tex) { m_OutTex = _Tex; }
    void SetColor(const Vec3& _RGB) { SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::VEC4_0, &_RGB); }
};
